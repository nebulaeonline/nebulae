// Copyright (c) 2003-2019 Nebulae Foundation. All rights reserved.
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice, 
// this list of conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice, 
// this list of conditions and the following disclaimer in the documentation 
// and/or other materials provided with the distribution.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
// POSSIBILITY OF SUCH DAMAGE.

#include <Library/BaseLib.h>
#include <Library/UefiLib.h>
#include <Library/BaseMemoryLib.h>

#include "../../include/k0.h"
#include "../../include/arch/x64/x64.h"
#include "../../include/arch/uefi/memory.h"

// The number of bytes we reserve for the system
extern UINTN nebulae_system_table_reserved_bytes;

x64_cpu cpu;
x64_tss *tss;
x64_seg_descr *gdt;

// Zeros a 512-entry pml4 table representing a virtual address space
nebStatus x64ClearVirtualAddressSpace(x64_virtual_address_space *vas) {
    
    if (ISNULL(vas)) {
        return NEBERROR_NULL_PTR_UNEXPECTED;
    }

    ZeroMem(vas, sizeof(x64_pml4e) * X64_PAGING_TABLE_MAX);

    return NEB_OK;
}

// Executes various cpuinfo leaf functions
// and obtains the bit counts for the linear
// and physical address space
VOID x64InitCPU() {

    // First call cpuid with eax == 0x00
    // this call returns both the maximum
    // supported eax value in eax, along 
    // with the processor vendor string
    // in EBX, ECX & EDX
    EFI_STATUS cpuid_result = EFI_SUCCESS;

    // CpuId
    cpuid_result = AsmCpuid(0x00,
        &cpu.cpuinfo[0].reg[X64_REG_EAX],
        &cpu.cpuinfo[0].reg[X64_REG_EBX],
        &cpu.cpuinfo[0].reg[X64_REG_ECX],
        &cpu.cpuinfo[0].reg[X64_REG_EDX]);

    if (EFI_ERROR(cpuid_result)) {
        kernel_panic("Unable to query the cpu: %r\n", cpuid_result);
    }

    // Record the max eax value
    cpu.max_cpuid_eax = cpu.cpuinfo[0].reg[X64_REG_EAX];

    // Similar to the previous logic, the cpuidex function
    // returns its max value in eax
    cpuid_result = AsmCpuidEx(0x80000000, 
        0,        
        &cpu.cpuinfo[3].reg[X64_REG_EAX],
        &cpu.cpuinfo[3].reg[X64_REG_EBX],
        &cpu.cpuinfo[3].reg[X64_REG_ECX],
        &cpu.cpuinfo[3].reg[X64_REG_EDX]);

    if (EFI_ERROR(cpuid_result)) {
        kernel_panic("Unable to extended query the cpu: %r\n", cpuid_result);
    }

    // Record the max eax values for the cpuid
    // family of functions
    cpu.max_cpuid_eax = cpu.cpuinfo[0].reg[X64_REG_EAX];
    cpu.max_cpuidex_eax = cpu.cpuinfo[3].reg[X64_REG_EAX];

    // cpuid eax == 0x01
    if (cpu.max_cpuid_eax >= 0x01) {
        cpuid_result = AsmCpuid(0x01,
            &cpu.cpuinfo[1].reg[X64_REG_EAX],
            &cpu.cpuinfo[1].reg[X64_REG_EBX],
            &cpu.cpuinfo[1].reg[X64_REG_ECX],
            &cpu.cpuinfo[1].reg[X64_REG_EDX]);

        if (EFI_ERROR(cpuid_result)) {
            kernel_panic("Unable to query the cpu (eax == 0x01): %r\n", cpuid_result);
        }
    }

    // cpuid eax == 0x02
    if (cpu.max_cpuid_eax >= 0x02) {
        cpuid_result = AsmCpuid(0x02,
            &cpu.cpuinfo[2].reg[X64_REG_EAX],
            &cpu.cpuinfo[2].reg[X64_REG_EBX],
            &cpu.cpuinfo[2].reg[X64_REG_ECX],
            &cpu.cpuinfo[2].reg[X64_REG_EDX]);

        if (EFI_ERROR(cpuid_result)) {
            kernel_panic("Unable to query the cpu (eax == 0x02): %r\n", cpuid_result);
        }
    }

    // cpuid eax == 0x03
    // PROCESSOR SERIAL NUMBER -- DISCONTINUED AFTER PENTIUM III

    // CpuidEx eax == [0x80000001]...[0x80000001 + N]
    UINT32 current_cpuidex_fn, cpuinfo_offset = 0;
    for (current_cpuidex_fn = 0x80000001; current_cpuidex_fn <= cpu.max_cpuidex_eax; ++current_cpuidex_fn) {
        
        cpuinfo_offset = current_cpuidex_fn - 0x7FFFFFFD;
        cpuid_result = AsmCpuid(current_cpuidex_fn,
            &cpu.cpuinfo[cpuinfo_offset].reg[X64_REG_EAX],
            &cpu.cpuinfo[cpuinfo_offset].reg[X64_REG_EBX],
            &cpu.cpuinfo[cpuinfo_offset].reg[X64_REG_ECX],
            &cpu.cpuinfo[cpuinfo_offset].reg[X64_REG_EDX]);

        if (EFI_ERROR(cpuid_result)) {
            kernel_panic("Unable to query the cpu (eax == 0x%x): %r\n", current_cpuidex_fn, cpuid_result);
        }
        else if (current_cpuidex_fn == 0x80000008) { // Record cpu bit counts
            cpu.physical_address_bits = (UINT16)(cpu.cpuinfo[cpuinfo_offset].reg[X64_REG_EAX] & 0x000000FF);
            cpu.linear_address_bits = (UINT16)((cpu.cpuinfo[cpuinfo_offset].reg[X64_REG_EAX] & 0x0000FF00) >> 8);
        }
    }

    // Make sure we got the bit counts for physical and linear 
    // address spaces
    if (cpu.physical_address_bits == 0 || cpu.linear_address_bits == 0) {
        kernel_panic(L"Unable to determine the physical and linear address bit counts of the cpu(s)!\n");
    }
}

// Function to parse the cpuinfo structs for 
// processor feature information
BOOLEAN x64ReadCpuinfoFlags(UINT64 flag) {
    UINT32 bit, cpuid, reg;

    bit = LO32(flag);
    cpuid = ((flag & X64_CPUID_MASK) >> 32);
    reg = ((flag & X64_CPUID_REG_MASK) >> 36);

    return CHECK_BIT(cpu.cpuinfo[cpuid].reg[reg], bit);
}

// Initialize the global descriptor table (GDT)
VOID x64InitGDT() {
    extern preboot_mem_block k0_boot_scratch_area;

    // Allocate for gdt
    gdt = kPrebootMalloc(&k0_boot_scratch_area, X64_GDT_MAX * sizeof(x64_seg_descr), ALIGN_16);
    x64_seg_sel gdt_sel = { .base = gdt, .limit = X64_GDT_MAX * sizeof(x64_seg_descr) };

    if (ISNULL(gdt)) {
        kernel_panic(L"Problem allocating memory for global descriptor table\n");
    }

    if (ZeroMem(gdt, X64_GDT_MAX * sizeof(x64_seg_descr)) != gdt) {
        kernel_panic(L"Problem allocating memory for global descriptor table - storage initialization failure\n");
    }

    // Allocate for TSS
    tss = (x64_tss*)kPrebootMalloc(&k0_boot_scratch_area, sizeof(x64_tss), ALIGN_16);

    if (ISNULL(tss)) {
        kernel_panic(L"Problem allocating memory for task state segment\n");
    }

    if (ZeroMem(tss, sizeof(x64_tss)) != tss) {
        kernel_panic(L"Problem allocating memory for task state segment - storage initialization failure\n");
    }

    // Set up the 64-bit tss
    //tss->

    // Copy the existing gdt.
    // In order to keep uefi around, it is 
    // imperative not to mess with its 
    // execution environment too much, too early
    x64_seg_sel pgdt = { .base = 0,.limit = 0 };
    x64ReadGdtr(&pgdt);

    if (pgdt.base == 0) {
        kernel_panic(L"Unable to read gdt reg\n");
    }

    x64_seg_descr *gdt_entry = (x64_seg_descr*)pgdt.base;
    UINT16 current_entry = 0;

    while (gdt_entry < (pgdt.base + pgdt.limit)) {
        gdt[++current_entry] = (x64_seg_descr)*gdt_entry;
        gdt_entry++;
    }
    
    // Now fill in our entries

    // gdt[n + 1] DPL0, DATA, WRITEABLE
    gdt[++current_entry] = SEG_DESCR_FORMAT_BASE_ADDR(X64_SEGMENT_BASE) |
        SEG_DESCR_FORMAT_LIMIT(X64_SEGMENT_LIMIT) |
        X64_SEG_DATA |
        X64_SEG_DATA_WRITEABLE |
        X64_SEG_DEFAULT32 |
        X64_SEG_NON_SYSTEM_SEGMENT |
        X64_SEG_DPL0 |
        X64_SEG_PRESENT |
        X64_SEG_LIMIT_IN_PAGES;
    // gdt[n + 2] DPL0, CODE, READABLE
    gdt[++current_entry] = SEG_DESCR_FORMAT_BASE_ADDR(X64_SEGMENT_BASE) |
        SEG_DESCR_FORMAT_LIMIT(X64_SEGMENT_LIMIT) |
        X64_SEG_CODE |
        X64_SEG_CODE_READABLE |
        X64_SEG_DEFAULT32 |
        X64_SEG_NON_SYSTEM_SEGMENT |
        X64_SEG_DPL0 |
        X64_SEG_PRESENT |
        X64_SEG_LIMIT_IN_PAGES;
    // gdt[n + 3] DPL0, CODE, READABLE, 64-bit
    gdt[++current_entry] = SEG_DESCR_FORMAT_BASE_ADDR(X64_SEGMENT_BASE) |
        SEG_DESCR_FORMAT_LIMIT(X64_SEGMENT_LIMIT) |
        X64_SEG_CODE |
        X64_SEG_CODE_READABLE |
        X64_SEG_64BIT |
        X64_SEG_NON_SYSTEM_SEGMENT |
        X64_SEG_DPL0 |
        X64_SEG_PRESENT |
        X64_SEG_LIMIT_IN_PAGES;
    // gdt[n + 4] DPL0, DATA, WRITEABLE, EXPAND-DOWN
    gdt[++current_entry] = SEG_DESCR_FORMAT_BASE_ADDR(X64_SEGMENT_BASE) |
        SEG_DESCR_FORMAT_LIMIT(X64_SEGMENT_LIMIT) |
        X64_SEG_DATA |
        X64_SEG_DATA_WRITEABLE |
        X64_SEG_DATA_EXPAND_DOWN |
        X64_SEG_DEFAULT32 |
        X64_SEG_NON_SYSTEM_SEGMENT |
        X64_SEG_DPL0 |
        X64_SEG_PRESENT |
        X64_SEG_LIMIT_IN_PAGES;
    // gdt[n + 5] DPL3, DATA, WRITEABLE
    gdt[++current_entry] = SEG_DESCR_FORMAT_BASE_ADDR(X64_SEGMENT_BASE) |
        SEG_DESCR_FORMAT_LIMIT(X64_SEGMENT_LIMIT) |
        X64_SEG_DATA |
        X64_SEG_DATA_WRITEABLE |
        X64_SEG_DEFAULT32 |
        X64_SEG_NON_SYSTEM_SEGMENT |
        X64_SEG_DPL3 |
        X64_SEG_PRESENT |
        X64_SEG_LIMIT_IN_PAGES;
    // gdt[n + 6] DPL3, CODE, READABLE
    gdt[++current_entry] = SEG_DESCR_FORMAT_BASE_ADDR(X64_SEGMENT_BASE) |
        SEG_DESCR_FORMAT_LIMIT(X64_SEGMENT_LIMIT) |
        X64_SEG_CODE |
        X64_SEG_CODE_READABLE |
        X64_SEG_DEFAULT32 |
        X64_SEG_NON_SYSTEM_SEGMENT |
        X64_SEG_DPL3 |
        X64_SEG_PRESENT |
        X64_SEG_LIMIT_IN_PAGES;
    // gdt[n + 7] DPL3, CODE, READABLE, 64-bit
    gdt[++current_entry] = SEG_DESCR_FORMAT_BASE_ADDR(X64_SEGMENT_BASE) |
        SEG_DESCR_FORMAT_LIMIT(X64_SEGMENT_LIMIT) |
        X64_SEG_CODE |
        X64_SEG_CODE_READABLE |
        X64_SEG_64BIT |
        X64_SEG_NON_SYSTEM_SEGMENT |
        X64_SEG_DPL3 |
        X64_SEG_PRESENT |
        X64_SEG_LIMIT_IN_PAGES;
    // gdt[n + 8] DPL3, DATA, WRITEABLE, EXPAND-DOWN
    gdt[++current_entry] = SEG_DESCR_FORMAT_BASE_ADDR(X64_SEGMENT_BASE) |
        SEG_DESCR_FORMAT_LIMIT(X64_SEGMENT_LIMIT) |
        X64_SEG_DATA |
        X64_SEG_DATA_WRITEABLE |
        X64_SEG_DATA_EXPAND_DOWN |
        X64_SEG_DEFAULT32 |
        X64_SEG_NON_SYSTEM_SEGMENT |
        X64_SEG_DPL3 |
        X64_SEG_PRESENT |
        X64_SEG_LIMIT_IN_PAGES;
    // 64-bit TSS descriptor (CPU0) - takes up two gdt spots
    gdt[++current_entry] = SEG_DESCR_FORMAT_BASE_ADDR(&tss) |
        SEG_DESCR_FORMAT_LIMIT(sizeof(x64_tss)) |
        SEG_DESCR_FORMAT_SYSTEM_TYPE(X64_TYPE_TSS_AVAILABLE) |
        X64_SEG_SYSTEM_SEGMENT |
        X64_SEG_DPL0 |
        X64_SEG_PRESENT;
    gdt[++current_entry] = HI32((UINT64)&tss);

    // Set our gdt limit
    gdt_sel.limit = ((current_entry + 1) * sizeof(x64_seg_descr));

    // Install the new gdt    
    x64DisableInterrupts();
    x64WriteGdtr(&gdt_sel);
    x64EnableInterrupts();
}

// Initializes the interrupt descriptor table (IDT)
VOID x64InitIDT() {
    extern preboot_mem_block k0_boot_scratch_area;

    x64_inttrap_gate *idt = kPrebootMalloc(&k0_boot_scratch_area, X64_INTERRUPT_MAX * sizeof(x64_inttrap_gate), ALIGN_4KB);
    
    if (ISNULL(idt)) {
        kernel_panic(L"Problem allocating memory for interrupt descriptor table\n");
    }

    if (ZeroMem(idt, X64_INTERRUPT_MAX * sizeof(x64_inttrap_gate)) != idt) {
        kernel_panic(L"Problem allocating memory for interrupt descriptor table - storage initialization failure\n");
    }
}

// Read CR3 to obtain the address of the PML4 Table
EFI_VIRTUAL_ADDRESS x64GetCurrentPML4TableAddr() {
    UINT64 cr3 = AsmReadCr3();
    return (cr3 & X64_4KB_ALIGN_MASK);
}

// Finds a random block of memory that begins on a 2MB boundary and zeroes it.
// This will be the location of our initial boot scratch area
VOID x64AllocateBootScratchArea() {

    // Memory Subsystem Vars
    extern VOID* kmem_largest_block;
    extern UINTN kmem_largest_block_size;
    extern UINTN kmem_total_page_count;

    extern preboot_mem_block k0_boot_scratch_area;

    // Allocate 2MB unless we need to map more than 256GB to start with
    UINTN bytes_to_alloc;
    if ((kmem_total_page_count * EFI_PAGE_SIZE) > SIZE_256GB) {
        bytes_to_alloc = SIZE_2MB;
    }
    else {
        bytes_to_alloc = SIZE_4MB;
    }

    // We are randomly choosing an area in the largest block of free conventional memory
    // this buffer is 2MB aligned
    EFI_PHYSICAL_ADDRESS addr = (EFI_PHYSICAL_ADDRESS*)(GetCSPRNG64((UINT64)kmem_largest_block,
        (UINT64)(kmem_largest_block + (kmem_largest_block_size - bytes_to_alloc))) & X64_2MB_ALIGN_MASK);
    
    if (!ISNULL(addr) && ZeroMem(addr, bytes_to_alloc) != addr) {
        kernel_panic(L"There was a problem initializing the kernel's boot scratch area!\n");
    }
    else if (ISNULL(addr)) {
        kernel_panic(L"There was a problem initializing the kernel's boot scratch area - isaac64 failed!\n");
    }
    else if (k0_VERBOSE_DEBUG) {
        Print(L"Boot scratch area allocated @ 0x%lx\n", addr);
    }

    // Initialize our boot scratch area struct
    if (InitPrebootMemBlock(&k0_boot_scratch_area, addr, bytes_to_alloc) != &k0_boot_scratch_area) {
        kernel_panic(L"There was a problem initializing the kernel's boot scratch area - preboot mem block allocation failed!\n");
    }

    // Remove the(se) page(s) from the physical free page stack
    INTN bytes_to_remove = bytes_to_alloc;
    nebStatus remove_boot_scratch_page_result = NEB_OK;
    UINT64 current_addr = addr;

    while ((bytes_to_remove -= (remove_boot_scratch_page_result = RemoveFreePageContainingAddr(current_addr))) > 0) {

        if (NEB_ERROR(remove_boot_scratch_page_result)) {
            kernel_panic(L"Unable to remove boot scratch pages from physical memory stacks: %ld\n",
                remove_boot_scratch_page_result);
        }
        else {
            if (k0_VERBOSE_DEBUG) {
                Print(L"Removed boot scratch page from physical memory stacks\n");
            }
        }

        current_addr += remove_boot_scratch_page_result;
    }
}

// Builds our initial kernel page table
VOID x64BuildInitialKernelPageTable() {
    extern preboot_mem_block k0_boot_scratch_area;
    extern x64_virtual_address_space k0_addr_space;
    extern UINTN kmem_total_page_count;

    UINTN page_dir_count = UDIV_UP((kmem_total_page_count * EFI_PAGE_SIZE), SIZE_1GB);
    if (k0_VERBOSE_DEBUG) {
        Print(L"Need to map %lu page directories\n", page_dir_count);
    }

    // allocate memory space for our kernel page table and zero the memory
    k0_addr_space.pml4 = (x64_pml4e*)kPrebootMalloc(&k0_boot_scratch_area, X64_PAGING_TABLE_MAX * sizeof(x64_pml4e), ALIGN_4KB);
    if (ISNULL(k0_addr_space.pml4)) {
        kernel_panic(L"Problem building initial kernel page tables - pml4 allocation\n");
    }

    if (ZeroMem(k0_addr_space.pml4, X64_PAGING_TABLE_MAX * sizeof(x64_pml4e)) != k0_addr_space.pml4) {
        kernel_panic(L"Problem building initial kernel page tables - pml4 storage initialization\n");
    }
    
    k0_addr_space.pdpt = (x64_pdpte*)kPrebootMalloc(&k0_boot_scratch_area, X64_PAGING_TABLE_MAX * sizeof(x64_pdpte), ALIGN_4KB);
    if (ISNULL(k0_addr_space.pdpt)) {
        kernel_panic(L"Problem building initial kernel page tables - pdpt allocation\n");
    }

    if (ZeroMem(k0_addr_space.pdpt, X64_PAGING_TABLE_MAX * sizeof(x64_pdpte)) != k0_addr_space.pdpt) {
        kernel_panic(L"Problem building initial kernel page tables - pdpt storage initialization\n");
    }

    k0_addr_space.pde = (x64_pde*)kPrebootMalloc(&k0_boot_scratch_area, X64_PAGING_TABLE_MAX * sizeof(x64_pde) * page_dir_count, ALIGN_4KB);
    if (ISNULL(k0_addr_space.pde)) {
        kernel_panic(L"Problem building initial kernel page tables - pde allocation\n");
    }

    if (ZeroMem(k0_addr_space.pde, X64_PAGING_TABLE_MAX * sizeof(x64_pde) * page_dir_count) != k0_addr_space.pde) {
        kernel_panic(L"Problem building initial kernel page tables - pde storage initialization\n");
    }

    UINT64 current_addr = 0x0;

    // Point the first pml4 entry at pdpt[0]
    k0_addr_space.pml4[0] = ((UINT64)k0_addr_space.pdpt & X64_4KB_ALIGN_MASK) |
        X64_PAGING_PRESENT |
        X64_PAGING_DATA_WRITEABLE |
        X64_PAGING_SUPERVISOR_MODE;

    UINTN i, j;

    // Set up 1 pdpte and 512 pdes per GB of physical memory (rounded up to the next 1GB)
    for (i = 0; i < page_dir_count; ++i) {
        k0_addr_space.pdpt[i] = ((UINT64)&k0_addr_space.pde[i * X64_PAGING_TABLE_MAX] & X64_4KB_ALIGN_MASK) |
            X64_PAGING_PRESENT |
            X64_PAGING_DATA_WRITEABLE |
            X64_PAGING_SUPERVISOR_MODE;

        for (j = 0; j < X64_PAGING_TABLE_MAX; ++j) {
            if (current_addr < (kmem_total_page_count * EFI_PAGE_SIZE)) {
                k0_addr_space.pde[(i * X64_PAGING_TABLE_MAX) + j] = current_addr |
                    X64_PAGING_PRESENT |
                    X64_PAGING_DATA_WRITEABLE |
                    X64_PAGING_SUPERVISOR_MODE |
                    X64_PAGING_IS_PAGES;
            }
            else {
                k0_addr_space.pde[(i * X64_PAGING_TABLE_MAX) + j] = current_addr |
                    X64_PAGING_SUPERVISOR_MODE |
                    X64_PAGING_IS_PAGES;
            }

            current_addr += SIZE_2MB;
        }
    }

    // At this point, we have identity mapped all physical memory using 2MB pages.
    // There are potentially areas of memory that are still split up into 4KB pages due
    // to hardware/device mappings or other reserved memory areas.  For these 4KB areas,
    // we are going to add an extra level to the paging structures by pointing their
    // page directory entries to page tables.
    EFI_MEMORY_DESCRIPTOR *memmap_entry = NULL;
    UINT64 memmap_entries = memmap.size / memmap.descr_size;
    VOID *memmap_iter = memmap.memory_map;

    for (i = 0; i < memmap_entries; i++) {
        memmap_entry = (EFI_MEMORY_DESCRIPTOR *)memmap_iter;

        current_addr = (UINT64)memmap_entry->PhysicalStart;
        UINT64 mem_block_end = current_addr + ((UINT64)memmap_entry->NumberOfPages * (UINT64)EFI_PAGE_SIZE);

        while (current_addr < mem_block_end) {
            UINT64 *stack_result = NULL;

            if ((current_addr % SIZE_2MB) == 0 && (current_addr + SIZE_2MB) <= mem_block_end) {
                // Already mapped 2MB pages for all physical memory!
                current_addr += SIZE_2MB;
                continue;
            }
            else if ((current_addr % SIZE_4KB) == 0 && (current_addr + SIZE_4KB) <= mem_block_end) {
                // We need to locate the current pde mapping a 2MB page that this 4KB page
                // is a part of
                UINTN pml4_index = PML4_INDEX(current_addr);
                UINTN pdpt_index = PAGE_DIR_PTR_INDEX(current_addr);
                UINTN pde_index = PAGE_DIR_INDEX(current_addr);
                UINTN pte_index = PAGE_TABLE_INDEX(current_addr);

                x64_pdpte *cur_pdpt = k0_addr_space.pml4[pml4_index] & X64_4KB_ALIGN_MASK;
                x64_pde   *cur_pd = cur_pdpt[pdpt_index] & X64_4KB_ALIGN_MASK;
                x64_pte   *cur_pt = NULL;

                // See if this entry already points to pages, or if we need to create a page
                // table for it
                if (CHECK_BIT(cur_pd[pde_index], X64_PAGING_IS_PAGES)) {
                    // This page directory currently maps a 2MB page, so we need to 
                    // allocate space for a page table (X64_PAGING_TABLE_MAX ptes) 
                    // and set the page mapping
                    cur_pt = kPrebootMalloc(&k0_boot_scratch_area, X64_PAGING_TABLE_MAX * sizeof(x64_pte), SIZE_4KB);
                    if (ISNULL(cur_pt)) {
                        kernel_panic(L"Problem building initial kernel page tables - 4KB pt allocation\n");
                    }
                    if (ZeroMem(cur_pt, X64_PAGING_TABLE_MAX * sizeof(x64_pte)) != cur_pt) {
                        kernel_panic(L"Problem building initial kernel page tables - 4KB pt storage initialization\n");
                    }

                    // Rewrite this pde, because it now points to a pte
                    cur_pd[pde_index] = (UINT64)cur_pt |
                        X64_PAGING_PRESENT |
                        X64_PAGING_DATA_WRITEABLE |
                        X64_PAGING_SUPERVISOR_MODE;
                }
                else {
                    cur_pt = cur_pd[pde_index] & X64_4KB_ALIGN_MASK;
                }

                // Now we know which page table this physical page should be in, and we have
                // made sure that a valid page table exists, so just add the page!
                cur_pt[pte_index] = (UINT64)current_addr |
                    X64_PAGING_PRESENT |
                    X64_PAGING_DATA_WRITEABLE |
                    X64_PAGING_SUPERVISOR_MODE;

                if (cur_pt[pte_index] == 0) {
                    kernel_panic(L"Attempted to assign zero pagetable!");
                }

                // Increment to the next page
                current_addr += SIZE_4KB;
            }
            else {
                Print(L"Memory address did not fit into paging scheme: 0x%lx\n", current_addr);
                kernel_panic(L"mem_block_end == 0x%lx\n", mem_block_end);
            }
        }

        // Iterate to the next memory map entry
        memmap_iter = (CHAR8*)memmap_iter + memmap.descr_size;        
    }

    // Fill in the "gap" pages from 640KB-1MB; Hi IBM!
    current_addr = 0xA0000;
    while (current_addr < SIZE_1MB) {
        UINTN pml4_index = PML4_INDEX(current_addr);
        UINTN pdpt_index = PAGE_DIR_PTR_INDEX(current_addr);
        UINTN pde_index = PAGE_DIR_INDEX(current_addr);
        UINTN pte_index = PAGE_TABLE_INDEX(current_addr);

        x64_pdpte *cur_pdpt = k0_addr_space.pml4[pml4_index] & X64_4KB_ALIGN_MASK;
        x64_pde   *cur_pd = cur_pdpt[pdpt_index] & X64_4KB_ALIGN_MASK;
        x64_pte   *cur_pt = cur_pd[pde_index] & X64_4KB_ALIGN_MASK;

        cur_pt[pte_index] = (UINT64)current_addr |
            X64_PAGING_PRESENT |
            X64_PAGING_DATA_WRITEABLE |
            X64_PAGING_SUPERVISOR_MODE;

        current_addr += SIZE_4KB;
    }

    // We still have to map the last 64KB of physical memory, which seems
    // to be missing from the uefi memory map, and thus is likely verboten
    current_addr = 0x1FFF0000;
    while (current_addr < SIZE_1MB) {
        UINTN pml4_index = PML4_INDEX(current_addr);
        UINTN pdpt_index = PAGE_DIR_PTR_INDEX(current_addr);
        UINTN pde_index = PAGE_DIR_INDEX(current_addr);
        UINTN pte_index = PAGE_TABLE_INDEX(current_addr);

        x64_pdpte *cur_pdpt = k0_addr_space.pml4[pml4_index] & X64_4KB_ALIGN_MASK;
        x64_pde   *cur_pd = cur_pdpt[pdpt_index] & X64_4KB_ALIGN_MASK;
        x64_pte   *cur_pt = cur_pd[pde_index] & X64_4KB_ALIGN_MASK;

        cur_pt[pte_index] = (UINT64)current_addr |
            X64_PAGING_PRESENT |
            X64_PAGING_DATA_WRITEABLE |
            X64_PAGING_SUPERVISOR_MODE;

        current_addr += SIZE_4KB;
    }

    if (k0_VERBOSE_DEBUG) {
        Print(L"Finished building page tables for 4KB pages\n");
        Print(L"Current PML4 == 0x%lx\n", x64GetCurrentPML4TableAddr());
        Print(L"Current memmap (0x%lx bytes) location == 0x%lx\n", memmap.size, memmap.memory_map);
        //Print(L"sizeof(EFI_MEMORY_DESCRIPTOR) == 0x%lx\n", sizeof(EFI_MEMORY_DESCRIPTOR));
        //Print(L"memory descriptor size (per uefi): 0x%lx\n", memmap.descr_size);
        //Print(L"total pages: 0x%lx\n", kmem_total_page_count);
        Print(L"Jumping to new page tables @ 0x%lx\n", k0_addr_space.pml4);
    }

    //x64WriteCR3(k0_addr_space.pml4);
}

// Dumpgs gdt to screen
VOID  x64DumpGdt() {
    x64_seg_sel pgdt = { .base = 0, .limit = 0 };
    x64ReadGdtr(&pgdt);

    if (pgdt.base == 0) {
        kernel_panic(L"Unable to read gdt reg\n");
    }

    x64_seg_descr *gdt_entry = (x64_seg_descr*)pgdt.base;
    while (gdt_entry < (pgdt.base + pgdt.limit)) {
        Print(L"gdt_entry 0x%lx == 0x%lx\n", ((UINT64)gdt_entry - (UINT64)pgdt.base) / sizeof(x64_seg_descr), (UINT64)*gdt_entry);
        gdt_entry++;
    }
    Print(L"gdt.base == 0x%lx / gdt.limit == 0x%lx\n", pgdt.base, pgdt.limit);
}

// Finds a random 'nebulae_system_table_reserved_bytes'-sized block of memory 
// that begins on a 4KB boundary and zeroes it.
VOID x64AllocateSystemStruct() {

    // Memory Subsystem Vars
    extern nebulae_system_table *system_table;
    extern UINTN kmem_conventional_pages;
    extern VOID* kmem_largest_block;
    extern UINTN kmem_largest_block_size;
    extern UINTN kmem_largest_block_page_count;

    // We are randomly choosing an area in the largest block of free conventional memory
    // this buffer is 4KB aligned no matter the page size #TODO #ONFIRE #FIX
    system_table = (EFI_PHYSICAL_ADDRESS*)(GetCSPRNG64((UINT64)kmem_largest_block,
        (UINT64)(kmem_largest_block + (kmem_largest_block_size - nebulae_system_table_reserved_bytes))) & X64_4KB_ALIGN_MASK);

    if (system_table != NULL && ZeroMem(system_table, nebulae_system_table_reserved_bytes) != system_table) {
        kernel_panic(L"There was a problem initializing the kernel's private memory area!\n");
    }
    
    if (k0_PAGETABLE_DEBUG) {
        Print(L"Page table entry for 0x%lx == 0x%lx\n", 
            system_table,
            *x64GetPageInfo(system_table));
    }
}

// This function will return either the curent page table
// information for the page that contains this address, 
// or if the address is 0, it will return the page size
// of the page containing the first page of the address space
UINT64* x64GetPageInfo(EFI_VIRTUAL_ADDRESS addr) {
    
    x64_pml4e *l4_table = NULL;

    if (k0_PAGETABLE_DEBUG) {
        Print(L"cr3 == 0x%lx\n", AsmReadCr3());
    }

    // Attempt to locate the pml4 table
    if (!(l4_table = (x64_pml4e*)x64GetCurrentPML4TableAddr())) {
        kernel_panic("Unable to locate address to PML4 data structures!\n");
    } else if (k0_PAGETABLE_DEBUG) {
        Print(L"PML4 Table found at 0x%lx == 0x%lx\n", l4_table, *l4_table);
    }
    
    if (k0_PAGETABLE_DEBUG) {
        Print(L"PML4Entry at PML4Table[0x%lx] found at 0x%lx == 0x%lx\n", 
            PML4_INDEX(addr), 
            &l4_table[PML4_INDEX(addr)], 
            l4_table[PML4_INDEX(addr)]);
    }

    if (l4_table[PML4_INDEX(addr)] == 0) {
        return NULL;
    }

    x64_pdpte *l3_table = l4_table[PML4_INDEX(addr)] & X64_4KB_ALIGN_MASK;

    if (l3_table[PAGE_DIR_PTR_INDEX(addr)] == 0) {
        return NULL;
    }

    if (k0_PAGETABLE_DEBUG) {
        Print(L"PDPT @ 0x%lx\n", l3_table);
        Print(L"PDPT[0x%lx] @ 0x%lx == 0x%lx\n", 
            PAGE_DIR_PTR_INDEX(addr), 
            &l3_table[PAGE_DIR_PTR_INDEX(addr)], 
            l3_table[PAGE_DIR_PTR_INDEX(addr)]);
    }

    if (CHECK_BIT(l3_table[PAGE_DIR_PTR_INDEX(addr)], X64_PAGING_IS_PAGES)) {
        if (k0_PAGETABLE_DEBUG) {
            Print(L"1GB pages found\n");
        }
        if (addr == 0) {
            return X64_1GB_ALIGN_MASK;
        }
        else {
            return &l3_table[PAGE_DIR_PTR_INDEX(addr)];
        }
    } 

    x64_pde *l2_table = l3_table[PAGE_DIR_PTR_INDEX(addr)] & X64_4KB_ALIGN_MASK;

    if (l2_table[PAGE_DIR_INDEX(addr)] == 0) {
        return 0;
    }

    if (k0_PAGETABLE_DEBUG) {
        Print(L"PD @ 0x%lx\n", l2_table);
        Print(L"PD[0x%lx] @ 0x%lx == 0x%lx\n", 
            PAGE_DIR_INDEX(addr), 
            &l2_table[PAGE_DIR_INDEX(addr)], 
            l2_table[PAGE_DIR_INDEX(addr)]);
    }

    if (CHECK_BIT(l2_table[PAGE_DIR_INDEX(addr)], X64_PAGING_IS_PAGES)) {
        if (k0_PAGETABLE_DEBUG) {
            Print(L"2MB pages found\n");
        }

        if (addr == 0) {
            return X64_2MB_ALIGN_MASK;
        }
        else {
            return &l2_table[PAGE_DIR_INDEX(addr)];
        }
    }

    x64_pte *l1_table = l2_table[PAGE_DIR_INDEX(addr)];

    if (l1_table[PAGE_TABLE_INDEX(addr)] == 0) {
        return 0;
    }

    if (k0_PAGETABLE_DEBUG) {
        Print(L"PT @ 0x%lx\n", l1_table);
        Print(L"PT[0x%lx] @ 0x%lx == 0x%lx\n", 
            PAGE_TABLE_INDEX(addr), 
            &l1_table[PAGE_TABLE_INDEX(addr)], 
            l1_table[PAGE_TABLE_INDEX(addr)]);

        Print(L"4KB pages found\n");
    }

    if (addr == 0) {
        return X64_4KB_ALIGN_MASK;
    }
    else {
        return &l1_table[PAGE_TABLE_INDEX(addr)];
    }
}
