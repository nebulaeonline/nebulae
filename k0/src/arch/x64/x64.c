// Copyright (c) 2005-2019 Nebulae Foundation. All rights reserved.
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
#include "../../include/arch/x64/local_apic.h"

// The number of bytes we reserve for the system
extern UINTN nebulae_system_table_reserved_bytes;

// Interrupts
extern VOID interrupt_0x00_wrapper();
extern VOID interrupt_0x01_wrapper();
extern VOID interrupt_0x02_wrapper();
extern VOID interrupt_0x03_wrapper();
extern VOID interrupt_0x04_wrapper();
extern VOID interrupt_0x05_wrapper();
extern VOID interrupt_0x06_wrapper();
extern VOID interrupt_0x07_wrapper();
extern VOID interrupt_0x08_wrapper();
extern VOID interrupt_0x09_wrapper();
extern VOID interrupt_0x0A_wrapper();
extern VOID interrupt_0x0B_wrapper();
extern VOID interrupt_0x0C_wrapper();
extern VOID interrupt_0x0D_wrapper();
extern VOID interrupt_0x0E_wrapper();

extern VOID interrupt_0x10_wrapper();
extern VOID interrupt_0x11_wrapper();
extern VOID interrupt_0x12_wrapper();
extern VOID interrupt_0x13_wrapper();
extern VOID interrupt_0x14_wrapper();
extern VOID interrupt_0x15_wrapper();
extern VOID interrupt_0x16_wrapper();
extern VOID interrupt_0x17_wrapper();
extern VOID interrupt_0x18_wrapper();
extern VOID interrupt_0x19_wrapper();
extern VOID interrupt_0x1A_wrapper();
extern VOID interrupt_0x1B_wrapper();
extern VOID interrupt_0x1C_wrapper();
extern VOID interrupt_0x1D_wrapper();
extern VOID interrupt_0x1E_wrapper();
extern VOID interrupt_0x1F_wrapper();

extern VOID interrupt_0x20_wrapper();
extern VOID interrupt_0x21_wrapper();
extern VOID interrupt_0x22_wrapper();
extern VOID interrupt_0x23_wrapper();
extern VOID interrupt_0x24_wrapper();
extern VOID interrupt_0x25_wrapper();
extern VOID interrupt_0x26_wrapper();
extern VOID interrupt_0x27_wrapper();
extern VOID interrupt_0x28_wrapper();
extern VOID interrupt_0x29_wrapper();
extern VOID interrupt_0x2A_wrapper();
extern VOID interrupt_0x2B_wrapper();
extern VOID interrupt_0x2C_wrapper();
extern VOID interrupt_0x2D_wrapper();
extern VOID interrupt_0x2E_wrapper();
extern VOID interrupt_0x2F_wrapper();

extern VOID interrupt_0x30_wrapper();
extern VOID interrupt_0x31_wrapper();
extern VOID interrupt_0x32_wrapper();
extern VOID interrupt_0x33_wrapper();
extern VOID interrupt_0x34_wrapper();
extern VOID interrupt_0x35_wrapper();
extern VOID interrupt_0x36_wrapper();
extern VOID interrupt_0x37_wrapper();
extern VOID interrupt_0x38_wrapper();
extern VOID interrupt_0x39_wrapper();
extern VOID interrupt_0x3A_wrapper();
extern VOID interrupt_0x3B_wrapper();
extern VOID interrupt_0x3C_wrapper();
extern VOID interrupt_0x3D_wrapper();
extern VOID interrupt_0x3E_wrapper();
extern VOID interrupt_0x3F_wrapper();

extern VOID interrupt_0x40_wrapper();
extern VOID interrupt_0x41_wrapper();
extern VOID interrupt_0x42_wrapper();
extern VOID interrupt_0x43_wrapper();
extern VOID interrupt_0x44_wrapper();
extern VOID interrupt_0x45_wrapper();
extern VOID interrupt_0x46_wrapper();
extern VOID interrupt_0x47_wrapper();
extern VOID interrupt_0x48_wrapper();
extern VOID interrupt_0x49_wrapper();
extern VOID interrupt_0x4A_wrapper();
extern VOID interrupt_0x4B_wrapper();
extern VOID interrupt_0x4C_wrapper();
extern VOID interrupt_0x4D_wrapper();
extern VOID interrupt_0x4E_wrapper();
extern VOID interrupt_0x4F_wrapper();

extern VOID interrupt_0x50_wrapper();
extern VOID interrupt_0x51_wrapper();
extern VOID interrupt_0x52_wrapper();
extern VOID interrupt_0x53_wrapper();
extern VOID interrupt_0x54_wrapper();
extern VOID interrupt_0x55_wrapper();
extern VOID interrupt_0x56_wrapper();
extern VOID interrupt_0x57_wrapper();
extern VOID interrupt_0x58_wrapper();
extern VOID interrupt_0x59_wrapper();
extern VOID interrupt_0x5A_wrapper();
extern VOID interrupt_0x5B_wrapper();
extern VOID interrupt_0x5C_wrapper();
extern VOID interrupt_0x5D_wrapper();
extern VOID interrupt_0x5E_wrapper();
extern VOID interrupt_0x5F_wrapper();

extern VOID interrupt_0x60_wrapper();
extern VOID interrupt_0x61_wrapper();
extern VOID interrupt_0x62_wrapper();
extern VOID interrupt_0x63_wrapper();
extern VOID interrupt_0x64_wrapper();
extern VOID interrupt_0x65_wrapper();
extern VOID interrupt_0x66_wrapper();
extern VOID interrupt_0x67_wrapper();
extern VOID interrupt_0x68_wrapper();
extern VOID interrupt_0x69_wrapper();
extern VOID interrupt_0x6A_wrapper();
extern VOID interrupt_0x6B_wrapper();
extern VOID interrupt_0x6C_wrapper();
extern VOID interrupt_0x6D_wrapper();
extern VOID interrupt_0x6E_wrapper();
extern VOID interrupt_0x6F_wrapper();

extern VOID interrupt_0x70_wrapper();
extern VOID interrupt_0x71_wrapper();
extern VOID interrupt_0x72_wrapper();
extern VOID interrupt_0x73_wrapper();
extern VOID interrupt_0x74_wrapper();
extern VOID interrupt_0x75_wrapper();
extern VOID interrupt_0x76_wrapper();
extern VOID interrupt_0x77_wrapper();
extern VOID interrupt_0x78_wrapper();
extern VOID interrupt_0x79_wrapper();
extern VOID interrupt_0x7A_wrapper();
extern VOID interrupt_0x7B_wrapper();
extern VOID interrupt_0x7C_wrapper();
extern VOID interrupt_0x7D_wrapper();
extern VOID interrupt_0x7E_wrapper();
extern VOID interrupt_0x7F_wrapper();

extern VOID interrupt_0x80_wrapper();
extern VOID interrupt_0x81_wrapper();
extern VOID interrupt_0x82_wrapper();
extern VOID interrupt_0x83_wrapper();
extern VOID interrupt_0x84_wrapper();
extern VOID interrupt_0x85_wrapper();
extern VOID interrupt_0x86_wrapper();
extern VOID interrupt_0x87_wrapper();
extern VOID interrupt_0x88_wrapper();
extern VOID interrupt_0x89_wrapper();
extern VOID interrupt_0x8A_wrapper();
extern VOID interrupt_0x8B_wrapper();
extern VOID interrupt_0x8C_wrapper();
extern VOID interrupt_0x8D_wrapper();
extern VOID interrupt_0x8E_wrapper();
extern VOID interrupt_0x8F_wrapper();

extern VOID interrupt_0x90_wrapper();
extern VOID interrupt_0x91_wrapper();
extern VOID interrupt_0x92_wrapper();
extern VOID interrupt_0x93_wrapper();
extern VOID interrupt_0x94_wrapper();
extern VOID interrupt_0x95_wrapper();
extern VOID interrupt_0x96_wrapper();
extern VOID interrupt_0x97_wrapper();
extern VOID interrupt_0x98_wrapper();
extern VOID interrupt_0x99_wrapper();
extern VOID interrupt_0x9A_wrapper();
extern VOID interrupt_0x9B_wrapper();
extern VOID interrupt_0x9C_wrapper();
extern VOID interrupt_0x9D_wrapper();
extern VOID interrupt_0x9E_wrapper();
extern VOID interrupt_0x9F_wrapper();

extern VOID interrupt_0xA0_wrapper();
extern VOID interrupt_0xA1_wrapper();
extern VOID interrupt_0xA2_wrapper();
extern VOID interrupt_0xA3_wrapper();
extern VOID interrupt_0xA4_wrapper();
extern VOID interrupt_0xA5_wrapper();
extern VOID interrupt_0xA6_wrapper();
extern VOID interrupt_0xA7_wrapper();
extern VOID interrupt_0xA8_wrapper();
extern VOID interrupt_0xA9_wrapper();
extern VOID interrupt_0xAA_wrapper();
extern VOID interrupt_0xAB_wrapper();
extern VOID interrupt_0xAC_wrapper();
extern VOID interrupt_0xAD_wrapper();
extern VOID interrupt_0xAE_wrapper();
extern VOID interrupt_0xAF_wrapper();

extern VOID interrupt_0xB0_wrapper();
extern VOID interrupt_0xB1_wrapper();
extern VOID interrupt_0xB2_wrapper();
extern VOID interrupt_0xB3_wrapper();
extern VOID interrupt_0xB4_wrapper();
extern VOID interrupt_0xB5_wrapper();
extern VOID interrupt_0xB6_wrapper();
extern VOID interrupt_0xB7_wrapper();
extern VOID interrupt_0xB8_wrapper();
extern VOID interrupt_0xB9_wrapper();
extern VOID interrupt_0xBA_wrapper();
extern VOID interrupt_0xBB_wrapper();
extern VOID interrupt_0xBC_wrapper();
extern VOID interrupt_0xBD_wrapper();
extern VOID interrupt_0xBE_wrapper();
extern VOID interrupt_0xBF_wrapper();

extern VOID interrupt_0xC0_wrapper();
extern VOID interrupt_0xC1_wrapper();
extern VOID interrupt_0xC2_wrapper();
extern VOID interrupt_0xC3_wrapper();
extern VOID interrupt_0xC4_wrapper();
extern VOID interrupt_0xC5_wrapper();
extern VOID interrupt_0xC6_wrapper();
extern VOID interrupt_0xC7_wrapper();
extern VOID interrupt_0xC8_wrapper();
extern VOID interrupt_0xC9_wrapper();
extern VOID interrupt_0xCA_wrapper();
extern VOID interrupt_0xCB_wrapper();
extern VOID interrupt_0xCC_wrapper();
extern VOID interrupt_0xCD_wrapper();
extern VOID interrupt_0xCE_wrapper();
extern VOID interrupt_0xCF_wrapper();

extern VOID interrupt_0xD0_wrapper();
extern VOID interrupt_0xD1_wrapper();
extern VOID interrupt_0xD2_wrapper();
extern VOID interrupt_0xD3_wrapper();
extern VOID interrupt_0xD4_wrapper();
extern VOID interrupt_0xD5_wrapper();
extern VOID interrupt_0xD6_wrapper();
extern VOID interrupt_0xD7_wrapper();
extern VOID interrupt_0xD8_wrapper();
extern VOID interrupt_0xD9_wrapper();
extern VOID interrupt_0xDA_wrapper();
extern VOID interrupt_0xDB_wrapper();
extern VOID interrupt_0xDC_wrapper();
extern VOID interrupt_0xDD_wrapper();
extern VOID interrupt_0xDE_wrapper();
extern VOID interrupt_0xDF_wrapper();

extern VOID interrupt_0xE0_wrapper();
extern VOID interrupt_0xE1_wrapper();
extern VOID interrupt_0xE2_wrapper();
extern VOID interrupt_0xE3_wrapper();
extern VOID interrupt_0xE4_wrapper();
extern VOID interrupt_0xE5_wrapper();
extern VOID interrupt_0xE6_wrapper();
extern VOID interrupt_0xE7_wrapper();
extern VOID interrupt_0xE8_wrapper();
extern VOID interrupt_0xE9_wrapper();
extern VOID interrupt_0xEA_wrapper();
extern VOID interrupt_0xEB_wrapper();
extern VOID interrupt_0xEC_wrapper();
extern VOID interrupt_0xED_wrapper();
extern VOID interrupt_0xEE_wrapper();
extern VOID interrupt_0xEF_wrapper();

extern VOID interrupt_0xF0_wrapper();
extern VOID interrupt_0xF1_wrapper();
extern VOID interrupt_0xF2_wrapper();
extern VOID interrupt_0xF3_wrapper();
extern VOID interrupt_0xF4_wrapper();
extern VOID interrupt_0xF5_wrapper();
extern VOID interrupt_0xF6_wrapper();
extern VOID interrupt_0xF7_wrapper();
extern VOID interrupt_0xF8_wrapper();
extern VOID interrupt_0xF9_wrapper();
extern VOID interrupt_0xFA_wrapper();
extern VOID interrupt_0xFB_wrapper();
extern VOID interrupt_0xFC_wrapper();
extern VOID interrupt_0xFD_wrapper();
extern VOID interrupt_0xFE_wrapper();
extern VOID interrupt_0xFF_wrapper();

x64_cpu cpu;
x64_tss *tss;
x64_seg_descr *gdt;

// Kernel stacks -- mostly IST
x64_kernel_stack kernel_stack[8];
BOOLEAN kernel_stacks_initialized = FALSE;

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
VOID x64InitBootCPU() {

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
    for (current_cpuidex_fn = 0x80000001; current_cpuidex_fn <= cpu.max_cpuidex_eax; current_cpuidex_fn++) {
        
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

    // If the kernel stacks have not been initialized yet, error out
    if (!kernel_stacks_initialized) {
        kernel_panic(L"Attempt to initialize gdt prior to initializing kernel stacks\n");
    }

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

    // Set up the 64-bit tss for CPU0
    tss->rsp[X64_PL0] = kernel_stack[0].stack_base;
    
    UINT32 i;
    for (i = 1; i < 8; i++) {
        tss->ist[i - 1] = kernel_stack[i].stack_base;
        Print(L"tss->ist[%u] == 0x%lx\n", i - 1, tss->ist[i - 1]);
    }
    tss->io_map_base_address = sizeof(x64_tss);
    Print(L"&tss == 0x%lx\n", tss);

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
    // Our entries always begin at
    // index 32 / offset 0x100
    current_entry = 31;

    // gdt[32] DPL0, DATA, WRITEABLE
    gdt[++current_entry] = SEG_DESCR_FORMAT_BASE_ADDR(X64_SEGMENT_BASE) |
        SEG_DESCR_FORMAT_LIMIT(X64_SEGMENT_LIMIT) |
        X64_SEG_DATA |
        X64_SEG_DATA_WRITEABLE |
        X64_SEG_DEFAULT32 |
        X64_SEG_NON_SYSTEM_SEGMENT |
        X64_SEG_DPL0 |
        X64_SEG_PRESENT |
        X64_SEG_LIMIT_IN_PAGES;
    // gdt[33] DPL0, CODE, READABLE
    gdt[++current_entry] = SEG_DESCR_FORMAT_BASE_ADDR(X64_SEGMENT_BASE) |
        SEG_DESCR_FORMAT_LIMIT(X64_SEGMENT_LIMIT) |
        X64_SEG_CODE |
        X64_SEG_CODE_READABLE |
        X64_SEG_DEFAULT32 |
        X64_SEG_NON_SYSTEM_SEGMENT |
        X64_SEG_DPL0 |
        X64_SEG_PRESENT |
        X64_SEG_LIMIT_IN_PAGES;
    // gdt[34] DPL0, CODE, READABLE, 64-bit
    gdt[++current_entry] = SEG_DESCR_FORMAT_BASE_ADDR(X64_SEGMENT_BASE) |
        SEG_DESCR_FORMAT_LIMIT(X64_SEGMENT_LIMIT) |
        X64_SEG_CODE |
        X64_SEG_CODE_READABLE |
        X64_SEG_64BIT |
        X64_SEG_NON_SYSTEM_SEGMENT |
        X64_SEG_DPL0 |
        X64_SEG_PRESENT |
        X64_SEG_LIMIT_IN_PAGES;
    // gdt[35] DPL3, DATA, WRITEABLE
    gdt[++current_entry] = SEG_DESCR_FORMAT_BASE_ADDR(X64_SEGMENT_BASE) |
        SEG_DESCR_FORMAT_LIMIT(X64_SEGMENT_LIMIT) |
        X64_SEG_DATA |
        X64_SEG_DATA_WRITEABLE |
        X64_SEG_DEFAULT32 |
        X64_SEG_NON_SYSTEM_SEGMENT |
        X64_SEG_DPL3 |
        X64_SEG_PRESENT |
        X64_SEG_LIMIT_IN_PAGES;
    // gdt[36] DPL3, CODE, READABLE
    gdt[++current_entry] = SEG_DESCR_FORMAT_BASE_ADDR(X64_SEGMENT_BASE) |
        SEG_DESCR_FORMAT_LIMIT(X64_SEGMENT_LIMIT) |
        X64_SEG_CODE |
        X64_SEG_CODE_READABLE |
        X64_SEG_DEFAULT32 |
        X64_SEG_NON_SYSTEM_SEGMENT |
        X64_SEG_DPL3 |
        X64_SEG_PRESENT |
        X64_SEG_LIMIT_IN_PAGES;
    // gdt[37] DPL3, CODE, READABLE, 64-bit
    gdt[++current_entry] = SEG_DESCR_FORMAT_BASE_ADDR(X64_SEGMENT_BASE) |
        SEG_DESCR_FORMAT_LIMIT(X64_SEGMENT_LIMIT) |
        X64_SEG_CODE |
        X64_SEG_CODE_READABLE |
        X64_SEG_64BIT |
        X64_SEG_NON_SYSTEM_SEGMENT |
        X64_SEG_DPL3 |
        X64_SEG_PRESENT |
        X64_SEG_LIMIT_IN_PAGES;
    // gdt[38] 64-bit TSS descriptor (CPU0) - takes up two gdt spots
    gdt[++current_entry] = SEG_DESCR_FORMAT_BASE_ADDR(tss) |
        SEG_DESCR_FORMAT_LIMIT(sizeof(x64_tss)) |
        SEG_DESCR_FORMAT_SYSTEM_TYPE(X64_TYPE_TSS_AVAILABLE) |
        X64_SEG_SYSTEM_SEGMENT |
        X64_SEG_DPL0 |
        X64_SEG_PRESENT;
    gdt[++current_entry] = HI32((UINT64)tss);

    // Set our gdt limit
    gdt_sel.limit = ((current_entry + 1) * sizeof(x64_seg_descr));

    // Install the new gdt    
    x64DisableInterrupts();
    x64WriteGdtr(&gdt_sel);
    x64LoadTR(CPU0_TSS_INDEX_GDT);
    x64EnableInterrupts();

    if (k0_VERBOSE_DEBUG) {
        Print(L"GDT initialized\n");
    }
}

// Initializes kernel stacks for boot CPU
// Must be called prior to initializing the GDT,
// because the GDT contains the TSS (and hence
// the IST) for the boot CPU
VOID x64InitKernelStacks() {
    extern preboot_mem_block k0_kernel_stack_area;

    VOID* kernel_stack_area_base_addr = x64AllocateRandomMemory(&k0_kernel_stack_area, SIZE_2MB, X64_4KB_ALIGN_MASK);
    if (ISNULL(kernel_stack_area_base_addr)) {
        kernel_panic(L"Could not allocate space for kernel stacks\n");
    }
    else if (k0_VERBOSE_DEBUG) {
        Print(L"Space allocated for kernel stacks\n");
    }

    UINT32 i;

    // The stack area is already 4KB aligned, and
    // allocation is in 4KB blocks, so requesting 
    // byte alignment here is ok
    for (i = 0; i < 8; i++) {
        kernel_stack[i].guard_page_low_base = kPrebootMalloc(&k0_kernel_stack_area, SIZE_4KB, 1);
        kernel_stack[i].stack_base = kPrebootMalloc(&k0_kernel_stack_area, SIZE_4KB * 8, 1);
        kernel_stack[i].guard_page_high_base = kPrebootMalloc(&k0_kernel_stack_area, SIZE_4KB, 1);
        kernel_stack[i].stack_base = kernel_stack[i].guard_page_high_base - ALIGN_16; 
    }

    // TODO
    // So now there's 1 regular stack + 7 interrupt stacks 
    // along with 16 guard pages, but the guard pages don't 
    // yet guard anything because we haven't moved to our own 
    // page tables yet :/
    kernel_stacks_initialized = TRUE;
}

// Initializes the interrupt descriptor table (IDT)
VOID x64InitIDT() {
    extern preboot_mem_block k0_boot_scratch_area;
    
    x64_inttrap_gate *idt = kPrebootMalloc(&k0_boot_scratch_area, 
        X64_INTERRUPT_MAX * sizeof(x64_inttrap_gate), 
        ALIGN_4KB);
    
    if (ISNULL(idt)) {
        kernel_panic(L"Problem allocating memory for interrupt descriptor table\n");
    }

    if (ZeroMem(idt, X64_INTERRUPT_MAX * sizeof(x64_inttrap_gate)) != idt) {
        kernel_panic(L"Problem allocating memory for interrupt descriptor table - storage initialization failure\n");
    }

    // idt[0]
    idt[0].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x00_wrapper);
    idt[0].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x00_wrapper);
    idt[0].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x00_wrapper);
    idt[0].ist = 1;
    idt[0].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[0].segment_selector = DPL0_CODE64_READABLE;

    // idt[1]
    idt[1].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x01_wrapper);
    idt[1].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x01_wrapper);
    idt[1].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x01_wrapper);
    idt[1].ist = 1;
    idt[1].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[1].segment_selector = DPL0_CODE64_READABLE;

    // idt[2]
    idt[2].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x02_wrapper);
    idt[2].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x02_wrapper);
    idt[2].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x02_wrapper);
    idt[2].ist = 1;
    idt[2].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[2].segment_selector = DPL0_CODE64_READABLE;

    // idt[3]
    idt[3].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x03_wrapper);
    idt[3].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x03_wrapper);
    idt[3].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x03_wrapper);
    idt[3].ist = 1;
    idt[3].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[3].segment_selector = DPL0_CODE64_READABLE;

    // idt[4]
    idt[4].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x04_wrapper);
    idt[4].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x04_wrapper);
    idt[4].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x04_wrapper);
    idt[4].ist = 1;
    idt[4].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[4].segment_selector = DPL0_CODE64_READABLE;

    // idt[5]
    idt[5].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x05_wrapper);
    idt[5].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x05_wrapper);
    idt[5].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x05_wrapper);
    idt[5].ist = 1;
    idt[5].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[5].segment_selector = DPL0_CODE64_READABLE;

    // idt[6]
    idt[6].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x06_wrapper);
    idt[6].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x06_wrapper);
    idt[6].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x06_wrapper);
    idt[6].ist = 1;
    idt[6].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[6].segment_selector = DPL0_CODE64_READABLE;

    // idt[7]
    idt[7].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x07_wrapper);
    idt[7].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x07_wrapper);
    idt[7].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x07_wrapper);
    idt[7].ist = 1;
    idt[7].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[7].segment_selector = DPL0_CODE64_READABLE;

    // idt[8]
    idt[8].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x08_wrapper);
    idt[8].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x08_wrapper);
    idt[8].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x08_wrapper);
    idt[8].ist = 1;
    idt[8].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[8].segment_selector = DPL0_CODE64_READABLE;

    // idt[9]
    idt[9].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x09_wrapper);
    idt[9].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x09_wrapper);
    idt[9].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x09_wrapper);
    idt[9].ist = 1;
    idt[9].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[9].segment_selector = DPL0_CODE64_READABLE;

    // idt[10]
    idt[10].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x0A_wrapper);
    idt[10].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x0A_wrapper);
    idt[10].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x0A_wrapper);
    idt[10].ist = 1;
    idt[10].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[10].segment_selector = DPL0_CODE64_READABLE;

    // idt[11]
    idt[11].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x0B_wrapper);
    idt[11].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x0B_wrapper);
    idt[11].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x0B_wrapper);
    idt[11].ist = 1;
    idt[11].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[11].segment_selector = DPL0_CODE64_READABLE;

    // idt[12]
    idt[12].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x0C_wrapper);
    idt[12].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x0C_wrapper);
    idt[12].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x0C_wrapper);
    idt[12].ist = 1;
    idt[12].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[12].segment_selector = DPL0_CODE64_READABLE;

    // idt[13]
    idt[13].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x0D_wrapper);
    idt[13].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x0D_wrapper);
    idt[13].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x0D_wrapper);
    idt[13].ist = 1;
    idt[13].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[13].segment_selector = DPL0_CODE64_READABLE;

    // idt[14]
    idt[14].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x0E_wrapper);
    idt[14].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x0E_wrapper);
    idt[14].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x0E_wrapper);
    idt[14].ist = 1;
    idt[14].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[14].segment_selector = DPL0_CODE64_READABLE;

    // idt[15]
    // Reserved

    // idt[16]
    idt[16].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x10_wrapper);
    idt[16].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x10_wrapper);
    idt[16].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x10_wrapper);
    idt[16].ist = 1;
    idt[16].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[16].segment_selector = DPL0_CODE64_READABLE;

    // idt[17]
    idt[17].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x11_wrapper);
    idt[17].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x11_wrapper);
    idt[17].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x11_wrapper);
    idt[17].ist = 1;
    idt[17].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[17].segment_selector = DPL0_CODE64_READABLE;

    // idt[18]
    idt[18].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x12_wrapper);
    idt[18].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x12_wrapper);
    idt[18].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x12_wrapper);
    idt[18].ist = 1;
    idt[18].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[18].segment_selector = DPL0_CODE64_READABLE;

    // idt[19]
    idt[19].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x13_wrapper);
    idt[19].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x13_wrapper);
    idt[19].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x13_wrapper);
    idt[19].ist = 1;
    idt[19].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[19].segment_selector = DPL0_CODE64_READABLE;

    // idt[20]
    idt[20].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x14_wrapper);
    idt[20].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x14_wrapper);
    idt[20].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x14_wrapper);
    idt[20].ist = 1;
    idt[20].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[20].segment_selector = DPL0_CODE64_READABLE;

    // idt[21] - idt[31] Intel Reserved

    // idt[32]
    idt[32].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x20_wrapper);
    idt[32].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x20_wrapper);
    idt[32].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x20_wrapper);
    idt[32].ist = 1;
    idt[32].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[32].segment_selector = DPL0_CODE64_READABLE;

    // idt[33]
    idt[33].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x21_wrapper);
    idt[33].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x21_wrapper);
    idt[33].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x21_wrapper);
    idt[33].ist = 1;
    idt[33].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[33].segment_selector = DPL0_CODE64_READABLE;

    // idt[34]
    idt[34].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x22_wrapper);
    idt[34].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x22_wrapper);
    idt[34].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x22_wrapper);
    idt[34].ist = 1;
    idt[34].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[34].segment_selector = DPL0_CODE64_READABLE;

    // idt[35]
    idt[35].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x23_wrapper);
    idt[35].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x23_wrapper);
    idt[35].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x23_wrapper);
    idt[35].ist = 1;
    idt[35].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[35].segment_selector = DPL0_CODE64_READABLE;

    // idt[36]
    idt[36].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x24_wrapper);
    idt[36].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x24_wrapper);
    idt[36].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x24_wrapper);
    idt[36].ist = 1;
    idt[36].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[36].segment_selector = DPL0_CODE64_READABLE;

    // idt[37]
    idt[37].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x25_wrapper);
    idt[37].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x25_wrapper);
    idt[37].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x25_wrapper);
    idt[37].ist = 1;
    idt[37].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[37].segment_selector = DPL0_CODE64_READABLE;

    // idt[38]
    idt[38].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x26_wrapper);
    idt[38].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x26_wrapper);
    idt[38].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x26_wrapper);
    idt[38].ist = 1;
    idt[38].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[38].segment_selector = DPL0_CODE64_READABLE;

    // idt[39]
    idt[39].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x27_wrapper);
    idt[39].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x27_wrapper);
    idt[39].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x27_wrapper);
    idt[39].ist = 1;
    idt[39].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[39].segment_selector = DPL0_CODE64_READABLE;

    // idt[40]
    idt[40].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x28_wrapper);
    idt[40].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x28_wrapper);
    idt[40].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x28_wrapper);
    idt[40].ist = 1;
    idt[40].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[40].segment_selector = DPL0_CODE64_READABLE;

    // idt[41]
    idt[41].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x29_wrapper);
    idt[41].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x29_wrapper);
    idt[41].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x29_wrapper);
    idt[41].ist = 1;
    idt[41].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[41].segment_selector = DPL0_CODE64_READABLE;

    // idt[42]
    idt[42].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x2A_wrapper);
    idt[42].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x2A_wrapper);
    idt[42].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x2A_wrapper);
    idt[42].ist = 1;
    idt[42].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[42].segment_selector = DPL0_CODE64_READABLE;

    // idt[43]
    idt[43].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x2B_wrapper);
    idt[43].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x2B_wrapper);
    idt[43].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x2B_wrapper);
    idt[43].ist = 1;
    idt[43].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[43].segment_selector = DPL0_CODE64_READABLE;

    // idt[44]
    idt[44].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x2C_wrapper);
    idt[44].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x2C_wrapper);
    idt[44].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x2C_wrapper);
    idt[44].ist = 1;
    idt[44].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[44].segment_selector = DPL0_CODE64_READABLE;

    // idt[45]
    idt[45].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x2D_wrapper);
    idt[45].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x2D_wrapper);
    idt[45].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x2D_wrapper);
    idt[45].ist = 1;
    idt[45].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[45].segment_selector = DPL0_CODE64_READABLE;

    // idt[46]
    idt[46].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x2E_wrapper);
    idt[46].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x2E_wrapper);
    idt[46].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x2E_wrapper);
    idt[46].ist = 1;
    idt[46].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[46].segment_selector = DPL0_CODE64_READABLE;

    // idt[47]
    idt[47].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x2F_wrapper);
    idt[47].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x2F_wrapper);
    idt[47].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x2F_wrapper);
    idt[47].ist = 1;
    idt[47].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[47].segment_selector = DPL0_CODE64_READABLE;

    // idt[48]
    idt[48].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x30_wrapper);
    idt[48].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x30_wrapper);
    idt[48].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x30_wrapper);
    idt[48].ist = 1;
    idt[48].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[48].segment_selector = DPL0_CODE64_READABLE;

    // idt[49]
    idt[49].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x31_wrapper);
    idt[49].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x31_wrapper);
    idt[49].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x31_wrapper);
    idt[49].ist = 1;
    idt[49].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[49].segment_selector = DPL0_CODE64_READABLE;

    // idt[50]
    idt[50].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x32_wrapper);
    idt[50].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x32_wrapper);
    idt[50].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x32_wrapper);
    idt[50].ist = 1;
    idt[50].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[50].segment_selector = DPL0_CODE64_READABLE;

    // idt[51]
    idt[51].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x33_wrapper);
    idt[51].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x33_wrapper);
    idt[51].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x33_wrapper);
    idt[51].ist = 1;
    idt[51].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[51].segment_selector = DPL0_CODE64_READABLE;

    // idt[52]
    idt[52].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x34_wrapper);
    idt[52].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x34_wrapper);
    idt[52].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x34_wrapper);
    idt[52].ist = 1;
    idt[52].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[52].segment_selector = DPL0_CODE64_READABLE;

    // idt[53]
    idt[53].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x35_wrapper);
    idt[53].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x35_wrapper);
    idt[53].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x35_wrapper);
    idt[53].ist = 1;
    idt[53].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[53].segment_selector = DPL0_CODE64_READABLE;

    // idt[54]
    idt[54].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x36_wrapper);
    idt[54].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x36_wrapper);
    idt[54].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x36_wrapper);
    idt[54].ist = 1;
    idt[54].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[54].segment_selector = DPL0_CODE64_READABLE;

    // idt[55]
    idt[55].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x37_wrapper);
    idt[55].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x37_wrapper);
    idt[55].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x37_wrapper);
    idt[55].ist = 1;
    idt[55].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[55].segment_selector = DPL0_CODE64_READABLE;

    // idt[56]
    idt[56].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x38_wrapper);
    idt[56].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x38_wrapper);
    idt[56].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x38_wrapper);
    idt[56].ist = 1;
    idt[56].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[56].segment_selector = DPL0_CODE64_READABLE;

    // idt[57]
    idt[57].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x39_wrapper);
    idt[57].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x39_wrapper);
    idt[57].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x39_wrapper);
    idt[57].ist = 1;
    idt[57].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[57].segment_selector = DPL0_CODE64_READABLE;

    // idt[58]
    idt[58].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x3A_wrapper);
    idt[58].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x3A_wrapper);
    idt[58].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x3A_wrapper);
    idt[58].ist = 1;
    idt[58].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[58].segment_selector = DPL0_CODE64_READABLE;

    // idt[59]
    idt[59].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x3B_wrapper);
    idt[59].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x3B_wrapper);
    idt[59].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x3B_wrapper);
    idt[59].ist = 1;
    idt[59].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[59].segment_selector = DPL0_CODE64_READABLE;

    // idt[60]
    idt[60].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x3C_wrapper);
    idt[60].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x3C_wrapper);
    idt[60].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x3C_wrapper);
    idt[60].ist = 1;
    idt[60].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[60].segment_selector = DPL0_CODE64_READABLE;

    // idt[61]
    idt[61].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x3D_wrapper);
    idt[61].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x3D_wrapper);
    idt[61].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x3D_wrapper);
    idt[61].ist = 1;
    idt[61].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[61].segment_selector = DPL0_CODE64_READABLE;

    // idt[62]
    idt[62].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x3E_wrapper);
    idt[62].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x3E_wrapper);
    idt[62].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x3E_wrapper);
    idt[62].ist = 1;
    idt[62].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[62].segment_selector = DPL0_CODE64_READABLE;

    // idt[63]
    idt[63].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x3F_wrapper);
    idt[63].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x3F_wrapper);
    idt[63].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x3F_wrapper);
    idt[63].ist = 1;
    idt[63].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[63].segment_selector = DPL0_CODE64_READABLE;

    // idt[64]
    idt[64].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x40_wrapper);
    idt[64].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x40_wrapper);
    idt[64].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x40_wrapper);
    idt[64].ist = 1;
    idt[64].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[64].segment_selector = DPL0_CODE64_READABLE;

    // idt[65]
    idt[65].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x41_wrapper);
    idt[65].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x41_wrapper);
    idt[65].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x41_wrapper);
    idt[65].ist = 1;
    idt[65].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[65].segment_selector = DPL0_CODE64_READABLE;

    // idt[66]
    idt[66].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x42_wrapper);
    idt[66].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x42_wrapper);
    idt[66].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x42_wrapper);
    idt[66].ist = 1;
    idt[66].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[66].segment_selector = DPL0_CODE64_READABLE;

    // idt[67]
    idt[67].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x43_wrapper);
    idt[67].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x43_wrapper);
    idt[67].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x43_wrapper);
    idt[67].ist = 1;
    idt[67].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[67].segment_selector = DPL0_CODE64_READABLE;

    // idt[68]
    idt[68].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x44_wrapper);
    idt[68].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x44_wrapper);
    idt[68].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x44_wrapper);
    idt[68].ist = 1;
    idt[68].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[68].segment_selector = DPL0_CODE64_READABLE;

    // idt[69]
    idt[69].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x45_wrapper);
    idt[69].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x45_wrapper);
    idt[69].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x45_wrapper);
    idt[69].ist = 1;
    idt[69].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[69].segment_selector = DPL0_CODE64_READABLE;

    // idt[70]
    idt[70].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x46_wrapper);
    idt[70].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x46_wrapper);
    idt[70].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x46_wrapper);
    idt[70].ist = 1;
    idt[70].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[70].segment_selector = DPL0_CODE64_READABLE;

    // idt[71]
    idt[71].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x47_wrapper);
    idt[71].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x47_wrapper);
    idt[71].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x47_wrapper);
    idt[71].ist = 1;
    idt[71].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[71].segment_selector = DPL0_CODE64_READABLE;

    // idt[72]
    idt[72].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x48_wrapper);
    idt[72].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x48_wrapper);
    idt[72].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x48_wrapper);
    idt[72].ist = 1;
    idt[72].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[72].segment_selector = DPL0_CODE64_READABLE;

    // idt[73]
    idt[73].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x49_wrapper);
    idt[73].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x49_wrapper);
    idt[73].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x49_wrapper);
    idt[73].ist = 1;
    idt[73].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[73].segment_selector = DPL0_CODE64_READABLE;

    // idt[74]
    idt[74].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x4A_wrapper);
    idt[74].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x4A_wrapper);
    idt[74].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x4A_wrapper);
    idt[74].ist = 1;
    idt[74].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[74].segment_selector = DPL0_CODE64_READABLE;

    // idt[75]
    idt[75].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x4B_wrapper);
    idt[75].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x4B_wrapper);
    idt[75].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x4B_wrapper);
    idt[75].ist = 1;
    idt[75].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[75].segment_selector = DPL0_CODE64_READABLE;

    // idt[76]
    idt[76].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x4C_wrapper);
    idt[76].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x4C_wrapper);
    idt[76].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x4C_wrapper);
    idt[76].ist = 1;
    idt[76].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[76].segment_selector = DPL0_CODE64_READABLE;

    // idt[77]
    idt[77].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x4D_wrapper);
    idt[77].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x4D_wrapper);
    idt[77].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x4D_wrapper);
    idt[77].ist = 1;
    idt[77].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[77].segment_selector = DPL0_CODE64_READABLE;

    // idt[78]
    idt[78].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x4E_wrapper);
    idt[78].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x4E_wrapper);
    idt[78].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x4E_wrapper);
    idt[78].ist = 1;
    idt[78].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[78].segment_selector = DPL0_CODE64_READABLE;

    // idt[79]
    idt[79].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x4F_wrapper);
    idt[79].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x4F_wrapper);
    idt[79].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x4F_wrapper);
    idt[79].ist = 1;
    idt[79].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[79].segment_selector = DPL0_CODE64_READABLE;

    // idt[80]
    idt[80].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x50_wrapper);
    idt[80].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x50_wrapper);
    idt[80].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x50_wrapper);
    idt[80].ist = 1;
    idt[80].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[80].segment_selector = DPL0_CODE64_READABLE;

    // idt[81]
    idt[81].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x51_wrapper);
    idt[81].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x51_wrapper);
    idt[81].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x51_wrapper);
    idt[81].ist = 1;
    idt[81].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[81].segment_selector = DPL0_CODE64_READABLE;

    // idt[82]
    idt[82].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x52_wrapper);
    idt[82].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x52_wrapper);
    idt[82].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x52_wrapper);
    idt[82].ist = 1;
    idt[82].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[82].segment_selector = DPL0_CODE64_READABLE;

    // idt[83]
    idt[83].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x53_wrapper);
    idt[83].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x53_wrapper);
    idt[83].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x53_wrapper);
    idt[83].ist = 1;
    idt[83].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[83].segment_selector = DPL0_CODE64_READABLE;

    // idt[84]
    idt[84].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x54_wrapper);
    idt[84].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x54_wrapper);
    idt[84].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x54_wrapper);
    idt[84].ist = 1;
    idt[84].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[84].segment_selector = DPL0_CODE64_READABLE;

    // idt[85]
    idt[85].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x55_wrapper);
    idt[85].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x55_wrapper);
    idt[85].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x55_wrapper);
    idt[85].ist = 1;
    idt[85].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[85].segment_selector = DPL0_CODE64_READABLE;

    // idt[86]
    idt[86].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x56_wrapper);
    idt[86].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x56_wrapper);
    idt[86].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x56_wrapper);
    idt[86].ist = 1;
    idt[86].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[86].segment_selector = DPL0_CODE64_READABLE;

    // idt[87]
    idt[87].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x57_wrapper);
    idt[87].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x57_wrapper);
    idt[87].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x57_wrapper);
    idt[87].ist = 1;
    idt[87].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[87].segment_selector = DPL0_CODE64_READABLE;

    // idt[88]
    idt[88].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x58_wrapper);
    idt[88].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x58_wrapper);
    idt[88].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x58_wrapper);
    idt[88].ist = 1;
    idt[88].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[88].segment_selector = DPL0_CODE64_READABLE;

    // idt[89]
    idt[89].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x59_wrapper);
    idt[89].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x59_wrapper);
    idt[89].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x59_wrapper);
    idt[89].ist = 1;
    idt[89].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[89].segment_selector = DPL0_CODE64_READABLE;

    // idt[90]
    idt[90].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x5A_wrapper);
    idt[90].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x5A_wrapper);
    idt[90].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x5A_wrapper);
    idt[90].ist = 1;
    idt[90].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[90].segment_selector = DPL0_CODE64_READABLE;

    // idt[91]
    idt[91].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x5B_wrapper);
    idt[91].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x5B_wrapper);
    idt[91].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x5B_wrapper);
    idt[91].ist = 1;
    idt[91].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[91].segment_selector = DPL0_CODE64_READABLE;

    // idt[92]
    idt[92].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x5C_wrapper);
    idt[92].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x5C_wrapper);
    idt[92].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x5C_wrapper);
    idt[92].ist = 1;
    idt[92].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[92].segment_selector = DPL0_CODE64_READABLE;

    // idt[93]
    idt[93].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x5D_wrapper);
    idt[93].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x5D_wrapper);
    idt[93].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x5D_wrapper);
    idt[93].ist = 1;
    idt[93].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[93].segment_selector = DPL0_CODE64_READABLE;

    // idt[94]
    idt[94].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x5E_wrapper);
    idt[94].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x5E_wrapper);
    idt[94].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x5E_wrapper);
    idt[94].ist = 1;
    idt[94].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[94].segment_selector = DPL0_CODE64_READABLE;

    // idt[95]
    idt[95].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x5F_wrapper);
    idt[95].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x5F_wrapper);
    idt[95].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x5F_wrapper);
    idt[95].ist = 1;
    idt[95].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[95].segment_selector = DPL0_CODE64_READABLE;

    // idt[96]
    idt[96].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x60_wrapper);
    idt[96].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x60_wrapper);
    idt[96].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x60_wrapper);
    idt[96].ist = 1;
    idt[96].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[96].segment_selector = DPL0_CODE64_READABLE;

    // idt[97]
    idt[97].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x61_wrapper);
    idt[97].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x61_wrapper);
    idt[97].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x61_wrapper);
    idt[97].ist = 1;
    idt[97].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[97].segment_selector = DPL0_CODE64_READABLE;

    // idt[98]
    idt[98].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x62_wrapper);
    idt[98].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x62_wrapper);
    idt[98].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x62_wrapper);
    idt[98].ist = 1;
    idt[98].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[98].segment_selector = DPL0_CODE64_READABLE;

    // idt[99]
    idt[99].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x63_wrapper);
    idt[99].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x63_wrapper);
    idt[99].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x63_wrapper);
    idt[99].ist = 1;
    idt[99].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[99].segment_selector = DPL0_CODE64_READABLE;

    // idt[100]
    idt[100].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x64_wrapper);
    idt[100].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x64_wrapper);
    idt[100].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x64_wrapper);
    idt[100].ist = 1;
    idt[100].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[100].segment_selector = DPL0_CODE64_READABLE;

    // idt[101]
    idt[101].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x65_wrapper);
    idt[101].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x65_wrapper);
    idt[101].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x65_wrapper);
    idt[101].ist = 1;
    idt[101].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[101].segment_selector = DPL0_CODE64_READABLE;

    // idt[102]
    idt[102].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x66_wrapper);
    idt[102].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x66_wrapper);
    idt[102].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x66_wrapper);
    idt[102].ist = 1;
    idt[102].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[102].segment_selector = DPL0_CODE64_READABLE;

    // idt[103]
    idt[103].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x67_wrapper);
    idt[103].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x67_wrapper);
    idt[103].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x67_wrapper);
    idt[103].ist = 1;
    idt[103].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[103].segment_selector = DPL0_CODE64_READABLE;

    // idt[104]
    idt[104].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x68_wrapper);
    idt[104].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x68_wrapper);
    idt[104].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x68_wrapper);
    idt[104].ist = 1;
    idt[104].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[104].segment_selector = DPL0_CODE64_READABLE;

    // idt[105]
    idt[105].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x69_wrapper);
    idt[105].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x69_wrapper);
    idt[105].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x69_wrapper);
    idt[105].ist = 1;
    idt[105].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[105].segment_selector = DPL0_CODE64_READABLE;

    // idt[106]
    idt[106].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x6A_wrapper);
    idt[106].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x6A_wrapper);
    idt[106].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x6A_wrapper);
    idt[106].ist = 1;
    idt[106].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[106].segment_selector = DPL0_CODE64_READABLE;

    // idt[107]
    idt[107].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x6B_wrapper);
    idt[107].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x6B_wrapper);
    idt[107].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x6B_wrapper);
    idt[107].ist = 1;
    idt[107].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[107].segment_selector = DPL0_CODE64_READABLE;

    // idt[108]
    idt[108].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x6C_wrapper);
    idt[108].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x6C_wrapper);
    idt[108].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x6C_wrapper);
    idt[108].ist = 1;
    idt[108].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[108].segment_selector = DPL0_CODE64_READABLE;

    // idt[109]
    idt[109].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x6D_wrapper);
    idt[109].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x6D_wrapper);
    idt[109].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x6D_wrapper);
    idt[109].ist = 1;
    idt[109].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[109].segment_selector = DPL0_CODE64_READABLE;

    // idt[110]
    idt[110].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x6E_wrapper);
    idt[110].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x6E_wrapper);
    idt[110].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x6E_wrapper);
    idt[110].ist = 1;
    idt[110].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[110].segment_selector = DPL0_CODE64_READABLE;

    // idt[111]
    idt[111].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x6F_wrapper);
    idt[111].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x6F_wrapper);
    idt[111].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x6F_wrapper);
    idt[111].ist = 1;
    idt[111].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[111].segment_selector = DPL0_CODE64_READABLE;

    // idt[112]
    idt[112].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x70_wrapper);
    idt[112].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x70_wrapper);
    idt[112].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x70_wrapper);
    idt[112].ist = 1;
    idt[112].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[112].segment_selector = DPL0_CODE64_READABLE;

    // idt[113]
    idt[113].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x71_wrapper);
    idt[113].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x71_wrapper);
    idt[113].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x71_wrapper);
    idt[113].ist = 1;
    idt[113].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[113].segment_selector = DPL0_CODE64_READABLE;

    // idt[114]
    idt[114].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x72_wrapper);
    idt[114].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x72_wrapper);
    idt[114].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x72_wrapper);
    idt[114].ist = 1;
    idt[114].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[114].segment_selector = DPL0_CODE64_READABLE;

    // idt[115]
    idt[115].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x73_wrapper);
    idt[115].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x73_wrapper);
    idt[115].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x73_wrapper);
    idt[115].ist = 1;
    idt[115].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[115].segment_selector = DPL0_CODE64_READABLE;

    // idt[116]
    idt[116].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x74_wrapper);
    idt[116].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x74_wrapper);
    idt[116].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x74_wrapper);
    idt[116].ist = 1;
    idt[116].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[116].segment_selector = DPL0_CODE64_READABLE;

    // idt[117]
    idt[117].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x75_wrapper);
    idt[117].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x75_wrapper);
    idt[117].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x75_wrapper);
    idt[117].ist = 1;
    idt[117].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[117].segment_selector = DPL0_CODE64_READABLE;

    // idt[118]
    idt[118].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x76_wrapper);
    idt[118].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x76_wrapper);
    idt[118].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x76_wrapper);
    idt[118].ist = 1;
    idt[118].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[118].segment_selector = DPL0_CODE64_READABLE;

    // idt[119]
    idt[119].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x77_wrapper);
    idt[119].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x77_wrapper);
    idt[119].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x77_wrapper);
    idt[119].ist = 1;
    idt[119].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[119].segment_selector = DPL0_CODE64_READABLE;

    // idt[120]
    idt[120].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x78_wrapper);
    idt[120].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x78_wrapper);
    idt[120].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x78_wrapper);
    idt[120].ist = 1;
    idt[120].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[120].segment_selector = DPL0_CODE64_READABLE;

    // idt[121]
    idt[121].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x79_wrapper);
    idt[121].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x79_wrapper);
    idt[121].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x79_wrapper);
    idt[121].ist = 1;
    idt[121].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[121].segment_selector = DPL0_CODE64_READABLE;

    // idt[122]
    idt[122].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x7A_wrapper);
    idt[122].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x7A_wrapper);
    idt[122].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x7A_wrapper);
    idt[122].ist = 1;
    idt[122].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[122].segment_selector = DPL0_CODE64_READABLE;

    // idt[123]
    idt[123].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x7B_wrapper);
    idt[123].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x7B_wrapper);
    idt[123].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x7B_wrapper);
    idt[123].ist = 1;
    idt[123].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[123].segment_selector = DPL0_CODE64_READABLE;

    // idt[124]
    idt[124].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x7C_wrapper);
    idt[124].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x7C_wrapper);
    idt[124].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x7C_wrapper);
    idt[124].ist = 1;
    idt[124].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[124].segment_selector = DPL0_CODE64_READABLE;

    // idt[125]
    idt[125].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x7D_wrapper);
    idt[125].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x7D_wrapper);
    idt[125].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x7D_wrapper);
    idt[125].ist = 1;
    idt[125].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[125].segment_selector = DPL0_CODE64_READABLE;

    // idt[126]
    idt[126].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x7E_wrapper);
    idt[126].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x7E_wrapper);
    idt[126].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x7E_wrapper);
    idt[126].ist = 1;
    idt[126].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[126].segment_selector = DPL0_CODE64_READABLE;

    // idt[127]
    idt[127].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x7F_wrapper);
    idt[127].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x7F_wrapper);
    idt[127].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x7F_wrapper);
    idt[127].ist = 1;
    idt[127].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[127].segment_selector = DPL0_CODE64_READABLE;

    // idt[128]
    idt[128].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x80_wrapper);
    idt[128].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x80_wrapper);
    idt[128].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x80_wrapper);
    idt[128].ist = 1;
    idt[128].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[128].segment_selector = DPL0_CODE64_READABLE;

    // idt[129]
    idt[129].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x81_wrapper);
    idt[129].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x81_wrapper);
    idt[129].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x81_wrapper);
    idt[129].ist = 1;
    idt[129].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[129].segment_selector = DPL0_CODE64_READABLE;

    // idt[130]
    idt[130].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x82_wrapper);
    idt[130].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x82_wrapper);
    idt[130].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x82_wrapper);
    idt[130].ist = 1;
    idt[130].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[130].segment_selector = DPL0_CODE64_READABLE;

    // idt[131]
    idt[131].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x83_wrapper);
    idt[131].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x83_wrapper);
    idt[131].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x83_wrapper);
    idt[131].ist = 1;
    idt[131].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[131].segment_selector = DPL0_CODE64_READABLE;

    // idt[132]
    idt[132].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x84_wrapper);
    idt[132].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x84_wrapper);
    idt[132].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x84_wrapper);
    idt[132].ist = 1;
    idt[132].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[132].segment_selector = DPL0_CODE64_READABLE;

    // idt[133]
    idt[133].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x85_wrapper);
    idt[133].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x85_wrapper);
    idt[133].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x85_wrapper);
    idt[133].ist = 1;
    idt[133].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[133].segment_selector = DPL0_CODE64_READABLE;

    // idt[134]
    idt[134].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x86_wrapper);
    idt[134].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x86_wrapper);
    idt[134].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x86_wrapper);
    idt[134].ist = 1;
    idt[134].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[134].segment_selector = DPL0_CODE64_READABLE;

    // idt[135]
    idt[135].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x87_wrapper);
    idt[135].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x87_wrapper);
    idt[135].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x87_wrapper);
    idt[135].ist = 1;
    idt[135].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[135].segment_selector = DPL0_CODE64_READABLE;

    // idt[136]
    idt[136].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x88_wrapper);
    idt[136].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x88_wrapper);
    idt[136].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x88_wrapper);
    idt[136].ist = 1;
    idt[136].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[136].segment_selector = DPL0_CODE64_READABLE;

    // idt[137]
    idt[137].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x89_wrapper);
    idt[137].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x89_wrapper);
    idt[137].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x89_wrapper);
    idt[137].ist = 1;
    idt[137].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[137].segment_selector = DPL0_CODE64_READABLE;

    // idt[138]
    idt[138].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x8A_wrapper);
    idt[138].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x8A_wrapper);
    idt[138].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x8A_wrapper);
    idt[138].ist = 1;
    idt[138].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[138].segment_selector = DPL0_CODE64_READABLE;

    // idt[139]
    idt[139].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x8B_wrapper);
    idt[139].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x8B_wrapper);
    idt[139].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x8B_wrapper);
    idt[139].ist = 1;
    idt[139].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[139].segment_selector = DPL0_CODE64_READABLE;

    // idt[140]
    idt[140].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x8C_wrapper);
    idt[140].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x8C_wrapper);
    idt[140].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x8C_wrapper);
    idt[140].ist = 1;
    idt[140].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[140].segment_selector = DPL0_CODE64_READABLE;

    // idt[141]
    idt[141].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x8D_wrapper);
    idt[141].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x8D_wrapper);
    idt[141].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x8D_wrapper);
    idt[141].ist = 1;
    idt[141].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[141].segment_selector = DPL0_CODE64_READABLE;

    // idt[142]
    idt[142].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x8E_wrapper);
    idt[142].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x8E_wrapper);
    idt[142].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x8E_wrapper);
    idt[142].ist = 1;
    idt[142].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[142].segment_selector = DPL0_CODE64_READABLE;

    // idt[143]
    idt[143].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x8F_wrapper);
    idt[143].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x8F_wrapper);
    idt[143].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x8F_wrapper);
    idt[143].ist = 1;
    idt[143].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[143].segment_selector = DPL0_CODE64_READABLE;

    // idt[144]
    idt[144].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x90_wrapper);
    idt[144].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x90_wrapper);
    idt[144].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x90_wrapper);
    idt[144].ist = 1;
    idt[144].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[144].segment_selector = DPL0_CODE64_READABLE;

    // idt[145]
    idt[145].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x91_wrapper);
    idt[145].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x91_wrapper);
    idt[145].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x91_wrapper);
    idt[145].ist = 1;
    idt[145].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[145].segment_selector = DPL0_CODE64_READABLE;

    // idt[146]
    idt[146].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x92_wrapper);
    idt[146].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x92_wrapper);
    idt[146].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x92_wrapper);
    idt[146].ist = 1;
    idt[146].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[146].segment_selector = DPL0_CODE64_READABLE;

    // idt[147]
    idt[147].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x93_wrapper);
    idt[147].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x93_wrapper);
    idt[147].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x93_wrapper);
    idt[147].ist = 1;
    idt[147].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[147].segment_selector = DPL0_CODE64_READABLE;

    // idt[148]
    idt[148].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x94_wrapper);
    idt[148].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x94_wrapper);
    idt[148].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x94_wrapper);
    idt[148].ist = 1;
    idt[148].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[148].segment_selector = DPL0_CODE64_READABLE;

    // idt[149]
    idt[149].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x95_wrapper);
    idt[149].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x95_wrapper);
    idt[149].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x95_wrapper);
    idt[149].ist = 1;
    idt[149].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[149].segment_selector = DPL0_CODE64_READABLE;

    // idt[150]
    idt[150].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x96_wrapper);
    idt[150].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x96_wrapper);
    idt[150].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x96_wrapper);
    idt[150].ist = 1;
    idt[150].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[150].segment_selector = DPL0_CODE64_READABLE;

    // idt[151]
    idt[151].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x97_wrapper);
    idt[151].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x97_wrapper);
    idt[151].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x97_wrapper);
    idt[151].ist = 1;
    idt[151].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[151].segment_selector = DPL0_CODE64_READABLE;

    // idt[152]
    idt[152].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x98_wrapper);
    idt[152].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x98_wrapper);
    idt[152].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x98_wrapper);
    idt[152].ist = 1;
    idt[152].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[152].segment_selector = DPL0_CODE64_READABLE;

    // idt[153]
    idt[153].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x99_wrapper);
    idt[153].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x99_wrapper);
    idt[153].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x99_wrapper);
    idt[153].ist = 1;
    idt[153].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[153].segment_selector = DPL0_CODE64_READABLE;

    // idt[154]
    idt[154].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x9A_wrapper);
    idt[154].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x9A_wrapper);
    idt[154].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x9A_wrapper);
    idt[154].ist = 1;
    idt[154].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[154].segment_selector = DPL0_CODE64_READABLE;

    // idt[155]
    idt[155].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x9B_wrapper);
    idt[155].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x9B_wrapper);
    idt[155].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x9B_wrapper);
    idt[155].ist = 1;
    idt[155].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[155].segment_selector = DPL0_CODE64_READABLE;

    // idt[156]
    idt[156].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x9C_wrapper);
    idt[156].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x9C_wrapper);
    idt[156].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x9C_wrapper);
    idt[156].ist = 1;
    idt[156].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[156].segment_selector = DPL0_CODE64_READABLE;

    // idt[157]
    idt[157].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x9D_wrapper);
    idt[157].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x9D_wrapper);
    idt[157].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x9D_wrapper);
    idt[157].ist = 1;
    idt[157].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[157].segment_selector = DPL0_CODE64_READABLE;

    // idt[158]
    idt[158].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x9E_wrapper);
    idt[158].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x9E_wrapper);
    idt[158].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x9E_wrapper);
    idt[158].ist = 1;
    idt[158].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[158].segment_selector = DPL0_CODE64_READABLE;

    // idt[159]
    idt[159].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0x9F_wrapper);
    idt[159].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0x9F_wrapper);
    idt[159].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0x9F_wrapper);
    idt[159].ist = 1;
    idt[159].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[159].segment_selector = DPL0_CODE64_READABLE;

    // idt[160]
    idt[160].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xA0_wrapper);
    idt[160].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xA0_wrapper);
    idt[160].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xA0_wrapper);
    idt[160].ist = 1;
    idt[160].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[160].segment_selector = DPL0_CODE64_READABLE;

    // idt[161]
    idt[161].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xA1_wrapper);
    idt[161].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xA1_wrapper);
    idt[161].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xA1_wrapper);
    idt[161].ist = 1;
    idt[161].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[161].segment_selector = DPL0_CODE64_READABLE;

    // idt[162]
    idt[162].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xA2_wrapper);
    idt[162].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xA2_wrapper);
    idt[162].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xA2_wrapper);
    idt[162].ist = 1;
    idt[162].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[162].segment_selector = DPL0_CODE64_READABLE;

    // idt[163]
    idt[163].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xA3_wrapper);
    idt[163].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xA3_wrapper);
    idt[163].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xA3_wrapper);
    idt[163].ist = 1;
    idt[163].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[163].segment_selector = DPL0_CODE64_READABLE;

    // idt[164]
    idt[164].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xA4_wrapper);
    idt[164].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xA4_wrapper);
    idt[164].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xA4_wrapper);
    idt[164].ist = 1;
    idt[164].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[164].segment_selector = DPL0_CODE64_READABLE;

    // idt[165]
    idt[165].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xA5_wrapper);
    idt[165].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xA5_wrapper);
    idt[165].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xA5_wrapper);
    idt[165].ist = 1;
    idt[165].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[165].segment_selector = DPL0_CODE64_READABLE;

    // idt[166]
    idt[166].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xA6_wrapper);
    idt[166].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xA6_wrapper);
    idt[166].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xA6_wrapper);
    idt[166].ist = 1;
    idt[166].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[166].segment_selector = DPL0_CODE64_READABLE;

    // idt[167]
    idt[167].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xA7_wrapper);
    idt[167].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xA7_wrapper);
    idt[167].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xA7_wrapper);
    idt[167].ist = 1;
    idt[167].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[167].segment_selector = DPL0_CODE64_READABLE;

    // idt[168]
    idt[168].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xA8_wrapper);
    idt[168].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xA8_wrapper);
    idt[168].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xA8_wrapper);
    idt[168].ist = 1;
    idt[168].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[168].segment_selector = DPL0_CODE64_READABLE;

    // idt[169]
    idt[169].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xA9_wrapper);
    idt[169].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xA9_wrapper);
    idt[169].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xA9_wrapper);
    idt[169].ist = 1;
    idt[169].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[169].segment_selector = DPL0_CODE64_READABLE;

    // idt[170]
    idt[170].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xAA_wrapper);
    idt[170].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xAA_wrapper);
    idt[170].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xAA_wrapper);
    idt[170].ist = 1;
    idt[170].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[170].segment_selector = DPL0_CODE64_READABLE;

    // idt[171]
    idt[171].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xAB_wrapper);
    idt[171].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xAB_wrapper);
    idt[171].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xAB_wrapper);
    idt[171].ist = 1;
    idt[171].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[171].segment_selector = DPL0_CODE64_READABLE;

    // idt[172]
    idt[172].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xAC_wrapper);
    idt[172].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xAC_wrapper);
    idt[172].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xAC_wrapper);
    idt[172].ist = 1;
    idt[172].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[172].segment_selector = DPL0_CODE64_READABLE;

    // idt[173]
    idt[173].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xAD_wrapper);
    idt[173].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xAD_wrapper);
    idt[173].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xAD_wrapper);
    idt[173].ist = 1;
    idt[173].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[173].segment_selector = DPL0_CODE64_READABLE;

    // idt[174]
    idt[174].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xAE_wrapper);
    idt[174].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xAE_wrapper);
    idt[174].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xAE_wrapper);
    idt[174].ist = 1;
    idt[174].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[174].segment_selector = DPL0_CODE64_READABLE;

    // idt[175]
    idt[175].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xAF_wrapper);
    idt[175].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xAF_wrapper);
    idt[175].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xAF_wrapper);
    idt[175].ist = 1;
    idt[175].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[175].segment_selector = DPL0_CODE64_READABLE;

    // idt[176]
    idt[176].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xB0_wrapper);
    idt[176].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xB0_wrapper);
    idt[176].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xB0_wrapper);
    idt[176].ist = 1;
    idt[176].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[176].segment_selector = DPL0_CODE64_READABLE;

    // idt[177]
    idt[177].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xB1_wrapper);
    idt[177].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xB1_wrapper);
    idt[177].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xB1_wrapper);
    idt[177].ist = 1;
    idt[177].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[177].segment_selector = DPL0_CODE64_READABLE;

    // idt[178]
    idt[178].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xB2_wrapper);
    idt[178].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xB2_wrapper);
    idt[178].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xB2_wrapper);
    idt[178].ist = 1;
    idt[178].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[178].segment_selector = DPL0_CODE64_READABLE;

    // idt[179]
    idt[179].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xB3_wrapper);
    idt[179].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xB3_wrapper);
    idt[179].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xB3_wrapper);
    idt[179].ist = 1;
    idt[179].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[179].segment_selector = DPL0_CODE64_READABLE;

    // idt[180]
    idt[180].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xB4_wrapper);
    idt[180].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xB4_wrapper);
    idt[180].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xB4_wrapper);
    idt[180].ist = 1;
    idt[180].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[180].segment_selector = DPL0_CODE64_READABLE;

    // idt[181]
    idt[181].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xB5_wrapper);
    idt[181].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xB5_wrapper);
    idt[181].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xB5_wrapper);
    idt[181].ist = 1;
    idt[181].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[181].segment_selector = DPL0_CODE64_READABLE;

    // idt[182]
    idt[182].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xB6_wrapper);
    idt[182].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xB6_wrapper);
    idt[182].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xB6_wrapper);
    idt[182].ist = 1;
    idt[182].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[182].segment_selector = DPL0_CODE64_READABLE;

    // idt[183]
    idt[183].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xB7_wrapper);
    idt[183].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xB7_wrapper);
    idt[183].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xB7_wrapper);
    idt[183].ist = 1;
    idt[183].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[183].segment_selector = DPL0_CODE64_READABLE;

    // idt[184]
    idt[184].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xB8_wrapper);
    idt[184].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xB8_wrapper);
    idt[184].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xB8_wrapper);
    idt[184].ist = 1;
    idt[184].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[184].segment_selector = DPL0_CODE64_READABLE;

    // idt[185]
    idt[185].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xB9_wrapper);
    idt[185].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xB9_wrapper);
    idt[185].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xB9_wrapper);
    idt[185].ist = 1;
    idt[185].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[185].segment_selector = DPL0_CODE64_READABLE;

    // idt[186]
    idt[186].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xBA_wrapper);
    idt[186].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xBA_wrapper);
    idt[186].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xBA_wrapper);
    idt[186].ist = 1;
    idt[186].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[186].segment_selector = DPL0_CODE64_READABLE;

    // idt[187]
    idt[187].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xBB_wrapper);
    idt[187].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xBB_wrapper);
    idt[187].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xBB_wrapper);
    idt[187].ist = 1;
    idt[187].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[187].segment_selector = DPL0_CODE64_READABLE;

    // idt[188]
    idt[188].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xBC_wrapper);
    idt[188].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xBC_wrapper);
    idt[188].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xBC_wrapper);
    idt[188].ist = 1;
    idt[188].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[188].segment_selector = DPL0_CODE64_READABLE;

    // idt[189]
    idt[189].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xBD_wrapper);
    idt[189].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xBD_wrapper);
    idt[189].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xBD_wrapper);
    idt[189].ist = 1;
    idt[189].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[189].segment_selector = DPL0_CODE64_READABLE;

    // idt[190]
    idt[190].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xBE_wrapper);
    idt[190].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xBE_wrapper);
    idt[190].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xBE_wrapper);
    idt[190].ist = 1;
    idt[190].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[190].segment_selector = DPL0_CODE64_READABLE;

    // idt[191]
    idt[191].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xBF_wrapper);
    idt[191].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xBF_wrapper);
    idt[191].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xBF_wrapper);
    idt[191].ist = 1;
    idt[191].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[191].segment_selector = DPL0_CODE64_READABLE;

    // idt[192]
    idt[192].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xC0_wrapper);
    idt[192].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xC0_wrapper);
    idt[192].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xC0_wrapper);
    idt[192].ist = 1;
    idt[192].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[192].segment_selector = DPL0_CODE64_READABLE;

    // idt[193]
    idt[193].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xC1_wrapper);
    idt[193].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xC1_wrapper);
    idt[193].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xC1_wrapper);
    idt[193].ist = 1;
    idt[193].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[193].segment_selector = DPL0_CODE64_READABLE;

    // idt[194]
    idt[194].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xC2_wrapper);
    idt[194].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xC2_wrapper);
    idt[194].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xC2_wrapper);
    idt[194].ist = 1;
    idt[194].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[194].segment_selector = DPL0_CODE64_READABLE;

    // idt[195]
    idt[195].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xC3_wrapper);
    idt[195].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xC3_wrapper);
    idt[195].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xC3_wrapper);
    idt[195].ist = 1;
    idt[195].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[195].segment_selector = DPL0_CODE64_READABLE;

    // idt[196]
    idt[196].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xC4_wrapper);
    idt[196].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xC4_wrapper);
    idt[196].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xC4_wrapper);
    idt[196].ist = 1;
    idt[196].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[196].segment_selector = DPL0_CODE64_READABLE;

    // idt[197]
    idt[197].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xC5_wrapper);
    idt[197].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xC5_wrapper);
    idt[197].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xC5_wrapper);
    idt[197].ist = 1;
    idt[197].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[197].segment_selector = DPL0_CODE64_READABLE;

    // idt[198]
    idt[198].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xC6_wrapper);
    idt[198].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xC6_wrapper);
    idt[198].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xC6_wrapper);
    idt[198].ist = 1;
    idt[198].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[198].segment_selector = DPL0_CODE64_READABLE;

    // idt[199]
    idt[199].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xC7_wrapper);
    idt[199].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xC7_wrapper);
    idt[199].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xC7_wrapper);
    idt[199].ist = 1;
    idt[199].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[199].segment_selector = DPL0_CODE64_READABLE;

    // idt[200]
    idt[200].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xC8_wrapper);
    idt[200].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xC8_wrapper);
    idt[200].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xC8_wrapper);
    idt[200].ist = 1;
    idt[200].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[200].segment_selector = DPL0_CODE64_READABLE;

    // idt[201]
    idt[201].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xC9_wrapper);
    idt[201].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xC9_wrapper);
    idt[201].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xC9_wrapper);
    idt[201].ist = 1;
    idt[201].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[201].segment_selector = DPL0_CODE64_READABLE;

    // idt[202]
    idt[202].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xCA_wrapper);
    idt[202].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xCA_wrapper);
    idt[202].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xCA_wrapper);
    idt[202].ist = 1;
    idt[202].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[202].segment_selector = DPL0_CODE64_READABLE;

    // idt[203]
    idt[203].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xCB_wrapper);
    idt[203].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xCB_wrapper);
    idt[203].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xCB_wrapper);
    idt[203].ist = 1;
    idt[203].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[203].segment_selector = DPL0_CODE64_READABLE;

    // idt[204]
    idt[204].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xCC_wrapper);
    idt[204].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xCC_wrapper);
    idt[204].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xCC_wrapper);
    idt[204].ist = 1;
    idt[204].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[204].segment_selector = DPL0_CODE64_READABLE;

    // idt[205]
    idt[205].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xCD_wrapper);
    idt[205].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xCD_wrapper);
    idt[205].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xCD_wrapper);
    idt[205].ist = 1;
    idt[205].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[205].segment_selector = DPL0_CODE64_READABLE;

    // idt[206]
    idt[206].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xCE_wrapper);
    idt[206].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xCE_wrapper);
    idt[206].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xCE_wrapper);
    idt[206].ist = 1;
    idt[206].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[206].segment_selector = DPL0_CODE64_READABLE;

    // idt[207]
    idt[207].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xCF_wrapper);
    idt[207].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xCF_wrapper);
    idt[207].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xCF_wrapper);
    idt[207].ist = 1;
    idt[207].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[207].segment_selector = DPL0_CODE64_READABLE;

    // idt[208]
    idt[208].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xD0_wrapper);
    idt[208].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xD0_wrapper);
    idt[208].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xD0_wrapper);
    idt[208].ist = 1;
    idt[208].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[208].segment_selector = DPL0_CODE64_READABLE;

    // idt[209]
    idt[209].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xD1_wrapper);
    idt[209].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xD1_wrapper);
    idt[209].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xD1_wrapper);
    idt[209].ist = 1;
    idt[209].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[209].segment_selector = DPL0_CODE64_READABLE;

    // idt[210]
    idt[210].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xD2_wrapper);
    idt[210].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xD2_wrapper);
    idt[210].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xD2_wrapper);
    idt[210].ist = 1;
    idt[210].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[210].segment_selector = DPL0_CODE64_READABLE;

    // idt[211]
    idt[211].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xD3_wrapper);
    idt[211].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xD3_wrapper);
    idt[211].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xD3_wrapper);
    idt[211].ist = 1;
    idt[211].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[211].segment_selector = DPL0_CODE64_READABLE;

    // idt[212]
    idt[212].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xD4_wrapper);
    idt[212].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xD4_wrapper);
    idt[212].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xD4_wrapper);
    idt[212].ist = 1;
    idt[212].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[212].segment_selector = DPL0_CODE64_READABLE;

    // idt[213]
    idt[213].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xD5_wrapper);
    idt[213].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xD5_wrapper);
    idt[213].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xD5_wrapper);
    idt[213].ist = 1;
    idt[213].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[213].segment_selector = DPL0_CODE64_READABLE;

    // idt[214]
    idt[214].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xD6_wrapper);
    idt[214].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xD6_wrapper);
    idt[214].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xD6_wrapper);
    idt[214].ist = 1;
    idt[214].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[214].segment_selector = DPL0_CODE64_READABLE;

    // idt[215]
    idt[215].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xD7_wrapper);
    idt[215].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xD7_wrapper);
    idt[215].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xD7_wrapper);
    idt[215].ist = 1;
    idt[215].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[215].segment_selector = DPL0_CODE64_READABLE;

    // idt[216]
    idt[216].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xD8_wrapper);
    idt[216].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xD8_wrapper);
    idt[216].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xD8_wrapper);
    idt[216].ist = 1;
    idt[216].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[216].segment_selector = DPL0_CODE64_READABLE;

    // idt[217]
    idt[217].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xD9_wrapper);
    idt[217].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xD9_wrapper);
    idt[217].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xD9_wrapper);
    idt[217].ist = 1;
    idt[217].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[217].segment_selector = DPL0_CODE64_READABLE;

    // idt[218]
    idt[218].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xDA_wrapper);
    idt[218].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xDA_wrapper);
    idt[218].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xDA_wrapper);
    idt[218].ist = 1;
    idt[218].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[218].segment_selector = DPL0_CODE64_READABLE;

    // idt[219]
    idt[219].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xDB_wrapper);
    idt[219].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xDB_wrapper);
    idt[219].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xDB_wrapper);
    idt[219].ist = 1;
    idt[219].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[219].segment_selector = DPL0_CODE64_READABLE;

    // idt[220]
    idt[220].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xDC_wrapper);
    idt[220].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xDC_wrapper);
    idt[220].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xDC_wrapper);
    idt[220].ist = 1;
    idt[220].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[220].segment_selector = DPL0_CODE64_READABLE;

    // idt[221]
    idt[221].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xDD_wrapper);
    idt[221].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xDD_wrapper);
    idt[221].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xDD_wrapper);
    idt[221].ist = 1;
    idt[221].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[221].segment_selector = DPL0_CODE64_READABLE;

    // idt[222]
    idt[222].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xDE_wrapper);
    idt[222].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xDE_wrapper);
    idt[222].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xDE_wrapper);
    idt[222].ist = 1;
    idt[222].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[222].segment_selector = DPL0_CODE64_READABLE;

    // idt[223]
    idt[223].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xDF_wrapper);
    idt[223].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xDF_wrapper);
    idt[223].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xDF_wrapper);
    idt[223].ist = 1;
    idt[223].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[223].segment_selector = DPL0_CODE64_READABLE;

    // idt[224]
    idt[224].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xE0_wrapper);
    idt[224].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xE0_wrapper);
    idt[224].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xE0_wrapper);
    idt[224].ist = 1;
    idt[224].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[224].segment_selector = DPL0_CODE64_READABLE;

    // idt[225]
    idt[225].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xE1_wrapper);
    idt[225].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xE1_wrapper);
    idt[225].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xE1_wrapper);
    idt[225].ist = 1;
    idt[225].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[225].segment_selector = DPL0_CODE64_READABLE;

    // idt[226]
    idt[226].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xE2_wrapper);
    idt[226].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xE2_wrapper);
    idt[226].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xE2_wrapper);
    idt[226].ist = 1;
    idt[226].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[226].segment_selector = DPL0_CODE64_READABLE;

    // idt[227]
    idt[227].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xE3_wrapper);
    idt[227].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xE3_wrapper);
    idt[227].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xE3_wrapper);
    idt[227].ist = 1;
    idt[227].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[227].segment_selector = DPL0_CODE64_READABLE;

    // idt[228]
    idt[228].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xE4_wrapper);
    idt[228].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xE4_wrapper);
    idt[228].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xE4_wrapper);
    idt[228].ist = 1;
    idt[228].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[228].segment_selector = DPL0_CODE64_READABLE;

    // idt[229]
    idt[229].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xE5_wrapper);
    idt[229].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xE5_wrapper);
    idt[229].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xE5_wrapper);
    idt[229].ist = 1;
    idt[229].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[229].segment_selector = DPL0_CODE64_READABLE;

    // idt[230]
    idt[230].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xE6_wrapper);
    idt[230].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xE6_wrapper);
    idt[230].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xE6_wrapper);
    idt[230].ist = 1;
    idt[230].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[230].segment_selector = DPL0_CODE64_READABLE;

    // idt[231]
    idt[231].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xE7_wrapper);
    idt[231].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xE7_wrapper);
    idt[231].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xE7_wrapper);
    idt[231].ist = 1;
    idt[231].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[231].segment_selector = DPL0_CODE64_READABLE;

    // idt[232]
    idt[232].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xE8_wrapper);
    idt[232].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xE8_wrapper);
    idt[232].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xE8_wrapper);
    idt[232].ist = 1;
    idt[232].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[232].segment_selector = DPL0_CODE64_READABLE;

    // idt[233]
    idt[233].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xE9_wrapper);
    idt[233].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xE9_wrapper);
    idt[233].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xE9_wrapper);
    idt[233].ist = 1;
    idt[233].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[233].segment_selector = DPL0_CODE64_READABLE;

    // idt[234]
    idt[234].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xEA_wrapper);
    idt[234].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xEA_wrapper);
    idt[234].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xEA_wrapper);
    idt[234].ist = 1;
    idt[234].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[234].segment_selector = DPL0_CODE64_READABLE;

    // idt[235]
    idt[235].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xEB_wrapper);
    idt[235].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xEB_wrapper);
    idt[235].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xEB_wrapper);
    idt[235].ist = 1;
    idt[235].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[235].segment_selector = DPL0_CODE64_READABLE;

    // idt[236]
    idt[236].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xEC_wrapper);
    idt[236].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xEC_wrapper);
    idt[236].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xEC_wrapper);
    idt[236].ist = 1;
    idt[236].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[236].segment_selector = DPL0_CODE64_READABLE;

    // idt[237]
    idt[237].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xED_wrapper);
    idt[237].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xED_wrapper);
    idt[237].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xED_wrapper);
    idt[237].ist = 1;
    idt[237].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[237].segment_selector = DPL0_CODE64_READABLE;

    // idt[238]
    idt[238].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xEE_wrapper);
    idt[238].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xEE_wrapper);
    idt[238].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xEE_wrapper);
    idt[238].ist = 1;
    idt[238].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[238].segment_selector = DPL0_CODE64_READABLE;

    // idt[239]
    idt[239].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xEF_wrapper);
    idt[239].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xEF_wrapper);
    idt[239].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xEF_wrapper);
    idt[239].ist = 1;
    idt[239].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[239].segment_selector = DPL0_CODE64_READABLE;

    // idt[240]
    idt[240].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xF0_wrapper);
    idt[240].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xF0_wrapper);
    idt[240].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xF0_wrapper);
    idt[240].ist = 1;
    idt[240].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[240].segment_selector = DPL0_CODE64_READABLE;

    // idt[241]
    idt[241].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xF1_wrapper);
    idt[241].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xF1_wrapper);
    idt[241].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xF1_wrapper);
    idt[241].ist = 1;
    idt[241].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[241].segment_selector = DPL0_CODE64_READABLE;

    // idt[242]
    idt[242].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xF2_wrapper);
    idt[242].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xF2_wrapper);
    idt[242].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xF2_wrapper);
    idt[242].ist = 1;
    idt[242].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[242].segment_selector = DPL0_CODE64_READABLE;

    // idt[243]
    idt[243].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xF3_wrapper);
    idt[243].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xF3_wrapper);
    idt[243].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xF3_wrapper);
    idt[243].ist = 1;
    idt[243].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[243].segment_selector = DPL0_CODE64_READABLE;

    // idt[244]
    idt[244].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xF4_wrapper);
    idt[244].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xF4_wrapper);
    idt[244].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xF4_wrapper);
    idt[244].ist = 1;
    idt[244].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[244].segment_selector = DPL0_CODE64_READABLE;

    // idt[245]
    idt[245].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xF5_wrapper);
    idt[245].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xF5_wrapper);
    idt[245].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xF5_wrapper);
    idt[245].ist = 1;
    idt[245].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[245].segment_selector = DPL0_CODE64_READABLE;

    // idt[246]
    idt[246].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xF6_wrapper);
    idt[246].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xF6_wrapper);
    idt[246].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xF6_wrapper);
    idt[246].ist = 1;
    idt[246].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[246].segment_selector = DPL0_CODE64_READABLE;

    // idt[247]
    idt[247].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xF7_wrapper);
    idt[247].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xF7_wrapper);
    idt[247].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xF7_wrapper);
    idt[247].ist = 1;
    idt[247].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[247].segment_selector = DPL0_CODE64_READABLE;

    // idt[248]
    idt[248].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xF8_wrapper);
    idt[248].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xF8_wrapper);
    idt[248].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xF8_wrapper);
    idt[248].ist = 1;
    idt[248].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[248].segment_selector = DPL0_CODE64_READABLE;

    // idt[249]
    idt[249].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xF9_wrapper);
    idt[249].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xF9_wrapper);
    idt[249].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xF9_wrapper);
    idt[249].ist = 1;
    idt[249].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[249].segment_selector = DPL0_CODE64_READABLE;

    // idt[250]
    idt[250].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xFA_wrapper);
    idt[250].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xFA_wrapper);
    idt[250].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xFA_wrapper);
    idt[250].ist = 1;
    idt[250].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[250].segment_selector = DPL0_CODE64_READABLE;

    // idt[251]
    idt[251].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xFB_wrapper);
    idt[251].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xFB_wrapper);
    idt[251].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xFB_wrapper);
    idt[251].ist = 1;
    idt[251].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[251].segment_selector = DPL0_CODE64_READABLE;

    // idt[252]
    idt[252].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xFC_wrapper);
    idt[252].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xFC_wrapper);
    idt[252].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xFC_wrapper);
    idt[252].ist = 1;
    idt[252].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[252].segment_selector = DPL0_CODE64_READABLE;

    // idt[253]
    idt[253].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xFD_wrapper);
    idt[253].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xFD_wrapper);
    idt[253].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xFD_wrapper);
    idt[253].ist = 1;
    idt[253].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[253].segment_selector = DPL0_CODE64_READABLE;

    // idt[254]
    idt[254].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xFE_wrapper);
    idt[254].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xFE_wrapper);
    idt[254].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xFE_wrapper);
    idt[254].ist = 1;
    idt[254].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[254].segment_selector = DPL0_CODE64_READABLE;

    // idt[255]
    idt[255].procedure_entry_offset_00_15 = X64_ADDR_00_15((UINT64)&interrupt_0xFF_wrapper);
    idt[255].procedure_entry_offset_16_31 = X64_ADDR_16_31((UINT64)&interrupt_0xFF_wrapper);
    idt[255].procedure_entry_offset_32_63 = X64_ADDR_32_63((UINT64)&interrupt_0xFF_wrapper);
    idt[255].ist = 1;
    idt[255].typeflags = X64_TYPE_INT_GATE | X64_INT_GATE_PRESENT;;
    idt[255].segment_selector = DPL0_CODE64_READABLE;

    x64_seg_sel idt_sel = { .base = idt, .limit = X64_INTERRUPT_MAX * sizeof(x64_inttrap_gate) };
    
    x64DisableInterrupts();
    x64WriteIdtr(&idt_sel);
    
    extern UINT64 bsp_apic_address;
    
    if (ISNULL(bsp_apic_address)) {
        kernel_panic(L"Boot service processor I/O apic not found after init\n");
    }

    
    // Mask the spurious interrupt register bit bit to begin receiving interrupts
    UINT32 interrupt_mask = ReadIOApic(bsp_apic_address, 0xF0);
    Print(L"interrupt_mask == %lu\n", interrupt_mask);
    WriteIOApic(bsp_apic_address, 0xF0, interrupt_mask | 0x100);

    // Away we go!
    x64EnableInterrupts();

    if (k0_VERBOSE_DEBUG) {
        Print(L"IDT initialized\n");
    }
}

// Read CR3 to obtain the address of the PML4 Table
EFI_VIRTUAL_ADDRESS x64GetCurrentPML4TableAddr() {
    UINT64 cr3 = AsmReadCr3();
    return (cr3 & X64_4KB_ALIGN_MASK);
}

// Allocates a block of random memory within
// the largest available conventional chunk
VOID* x64AllocateRandomMemory(preboot_mem_block *mb, UINT64 size, UINT64 alignment_mask) {

    // Memory Subsystem Vars
    extern VOID* kmem_largest_block;
    extern UINTN kmem_largest_block_size;
    extern UINTN kmem_total_page_count;

    // We are randomly choosing an area in the largest block of free conventional memory
    EFI_PHYSICAL_ADDRESS addr;
    UINT64 lowest_addr = (UINT64)kmem_largest_block;
    UINT64 highest_addr = (UINT64)kmem_largest_block + (UINT64)kmem_largest_block_size;
    
    addr = (EFI_PHYSICAL_ADDRESS*)(GetCSPRNG64(lowest_addr, highest_addr) & alignment_mask);
    
    Print(L"Generated random #: 0x%lx\n", addr);
    Print(L"kmem_largest_block == 0x%lx\n", kmem_largest_block);
    Print(L"kmem_largest_block_size == %lu\n", kmem_largest_block_size);
    Print(L"kmem_total_page_count == 0x%lx\n", kmem_total_page_count);
    Print(L"lowest_addr == 0x%lx\n", lowest_addr);
    Print(L"highest_addr == 0x%lx\n", highest_addr);
    

    while (addr == 0 || !IsPageFree_Preboot(addr)) {
        addr = (EFI_PHYSICAL_ADDRESS*)(GetCSPRNG64((UINT64)kmem_largest_block,
            (UINT64)(kmem_largest_block + (kmem_largest_block_size - size))) & alignment_mask);
        Print(L"Generated random #: 0x%lx\n", addr);
        kernel_panic(L"");
    }
    
    if (!ISNULL(addr) && ZeroMem(addr, size) != addr) {
        kernel_panic(L"There was a problem initializing a kernel scratch area!\n");
    }
    else if (ISNULL(addr)) {
        kernel_panic(L"There was a problem initializing a kernel scratch area - isaac64 failed!\n");
    }
    else if (k0_VERBOSE_DEBUG) {
        Print(L"Scratch area allocated @ 0x%lx\n", addr);
    }

    // Initialize our boot scratch area struct
    if (InitPrebootMemBlock(mb, addr, size) != mb) {
        kernel_panic(L"There was a problem initializing the kernel's boot scratch area - preboot mem block allocation failed!\n");
    }

    // Remove the(se) page(s) from the physical free page stack
    INTN bytes_to_remove = size;
    nebStatus remove_scratch_page_result = NEB_OK;
    UINT64 current_addr = addr;

    while ((bytes_to_remove -= (remove_scratch_page_result = RemoveFreePageContainingAddr(current_addr))) > 0) {

        if (NEB_ERROR(remove_scratch_page_result)) {
            kernel_panic(L"Unable to remove scratch pages from physical memory stacks: %ld\n",
                remove_scratch_page_result);
        }
        else {
            if (k0_VERBOSE_DEBUG) {
                Print(L"Removed scratch page from physical memory stacks\n");
            }
        }

        current_addr += remove_scratch_page_result;
    }

    return mb->base_addr;
}

// Finds a random block of memory that begins on a 2MB boundary and zeroes it.
// This will be the location of our initial boot scratch area
VOID x64AllocateBootScratchArea() {

    // Memory Subsystem Vars
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

    VOID *bs = x64AllocateRandomMemory(&k0_boot_scratch_area, bytes_to_alloc, X64_2MB_ALIGN_MASK);
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
    for (i = 0; i < page_dir_count; i++) {
        k0_addr_space.pdpt[i] = ((UINT64)&k0_addr_space.pde[i * X64_PAGING_TABLE_MAX] & X64_4KB_ALIGN_MASK) |
            X64_PAGING_PRESENT |
            X64_PAGING_DATA_WRITEABLE |
            X64_PAGING_SUPERVISOR_MODE;

        for (j = 0; j < X64_PAGING_TABLE_MAX; j++) {
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
    extern preboot_mem_block k0_kernel_system_area;

    // We are randomly choosing an area in the largest block of free conventional memory
    // this buffer is 4KB aligned
    system_table = x64AllocateRandomMemory(&k0_kernel_system_area, nebulae_system_table_reserved_bytes, X64_4KB_ALIGN_MASK);

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
