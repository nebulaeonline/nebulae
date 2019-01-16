// Copyright (c) 2018-2019 Nebulae Foundation. All rights reserved.
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

// This 4KB structure represents a virtual address space mapping
typedef PACKED_MS struct s_x64_virtual_address_space {
    x64_pml4e pml4e[512];
} PACKED_GNU x64_virtual_address_space;

x64_cpu cpu;// This function zeros a 512-entry pml4 table
// representing a virtual address space
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

// Read CR3 to obtain the address of the PML4 Table
EFI_VIRTUAL_ADDRESS x64GetCurrentPML4TableAddr() {
    UINT64 cr3 = AsmReadCr3();
    return (cr3 & X64_4KB_ALIGN_MASK);
}

// Finds a random 8KB block of memory that begins on a 4KB boundary
// and zeroes it.
VOID x64AllocateSystemStruct() {

    // Memory Subsystem Vars
    extern EFI_PHYSICAL_ADDRESS* nebulae_system_table;
    extern UINTN kmem_conventional_pages;
    extern VOID* kmem_largest_block;
    extern UINTN kmem_largest_block_size;
    extern UINTN kmem_largest_block_page_count;

    // We are randomly choosing an 8KB area in the largest block of free conventional memory
    // this buffer is 4KB aligned no matter the page size
    nebulae_system_table = (EFI_PHYSICAL_ADDRESS*)(GetCSPRNG64((UINT64)kmem_largest_block, 
        (UINT64)(kmem_largest_block + (kmem_largest_block_size - SIZE_8KB))) & X64_4KB_ALIGN_MASK);

    if (nebulae_system_table != NULL && ZeroMem(nebulae_system_table, SIZE_8KB) != nebulae_system_table) {
        kernel_panic(L"There was a problem initializing the kernel's private memory area!\n");
    }
    
    if (k0_PAGETABLE_DEBUG) {
        Print(L"Page table entry for 0x%lx == 0x%lx\n", 
            nebulae_system_table, 
            *x64GetPageInfo(nebulae_system_table));
    }

    // Sanity checks
    *nebulae_system_table = 0xDEADBEEFULL;
    *(nebulae_system_table + sizeof(EFI_PHYSICAL_ADDRESS)) = L"HELLO";

    if (*nebulae_system_table != 0xDEADBEEFULL) {
        kernel_panic(L"Problem writing integer to memory location 0x%lx\n", nebulae_system_table);
    }
    else if (StrCmp(*(nebulae_system_table + sizeof(EFI_PHYSICAL_ADDRESS)), L"HELLO") != 0) {
        kernel_panic(L"Problem writing wide string to memory location 0x%lx\n", 
            (nebulae_system_table + sizeof(EFI_PHYSICAL_ADDRESS)));
    }
    
    // Clear the sanity check values
    *nebulae_system_table = 0x0ULL;
    SetMem16(nebulae_system_table + sizeof(EFI_PHYSICAL_ADDRESS), 6, 0);
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
