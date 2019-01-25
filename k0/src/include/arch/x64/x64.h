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

#ifndef __K0_X64_H
#define __K0_X64_H

#include <Library/UefiLib.h>
#include <X64/ProcessorBind.h>

#include "../../k0.h"

// Helpful constants
#define X64_REG_EAX                     0x00ULL
#define X64_REG_EBX                     0x01ULL
#define X64_REG_ECX                     0x02ULL
#define X64_REG_EDX                     0x03ULL

#define BIT0_64                         0x01ULL
#define BIT1_64                         (0x01ULL << 1)
#define BIT2_64                         (0x01ULL << 2)
#define BIT3_64                         (0x01ULL << 3)
#define BIT4_64                         (0x01ULL << 4)
#define BIT5_64                         (0x01ULL << 5)
#define BIT6_64                         (0x01ULL << 6)
#define BIT7_64                         (0x01ULL << 7)
#define BIT8_64                         (0x01ULL << 8)
#define BIT9_64                         (0x01ULL << 9)
#define BIT10_64                        (0x01ULL << 10)
#define BIT11_64                        (0x01ULL << 11)
#define BIT12_64                        (0x01ULL << 12)
#define BIT13_64                        (0x01ULL << 13)
#define BIT14_64                        (0x01ULL << 14)
#define BIT15_64                        (0x01ULL << 15)
#define BIT16_64                        (0x01ULL << 16)
#define BIT17_64                        (0x01ULL << 17)
#define BIT18_64                        (0x01ULL << 18)
#define BIT19_64                        (0x01ULL << 19)
#define BIT20_64                        (0x01ULL << 20)
#define BIT21_64                        (0x01ULL << 21)
#define BIT22_64                        (0x01ULL << 22)
#define BIT23_64                        (0x01ULL << 23)
#define BIT24_64                        (0x01ULL << 24)
#define BIT25_64                        (0x01ULL << 25)
#define BIT26_64                        (0x01ULL << 26)
#define BIT27_64                        (0x01ULL << 27)
#define BIT28_64                        (0x01ULL << 28)
#define BIT29_64                        (0x01ULL << 29)
#define BIT30_64                        (0x01ULL << 20)
#define BIT31_64                        (0x01ULL << 31)

// CPU feature flags
#define X64_CPUID_0x00                  0x00ULL
#define X64_CPUID_0x01                  (0x01ULL << 32)
#define X64_CPUID_0x02                  (0x02ULL << 32)
#define X64_CPUID_0x03                  (0x03ULL << 32)
#define X64_CPUID_0x04                  (0x04ULL << 32)
#define X64_CPUID_0x05                  (0x05ULL << 32)
#define X64_CPUID_0x06                  (0x06ULL << 32)
#define X64_CPUID_0x07                  (0x07ULL << 32)
#define X64_CPUID_0x08                  (0x08ULL << 32)
#define X64_CPUID_0x09                  (0x09ULL << 32)
#define X64_CPUID_0x0A                  (0x0AULL << 32)
#define X64_CPUID_MASK                  (0x0FULL << 32)

#define X64_CPUID_EAX                   0x00ULL
#define X64_CPUID_EBX                   (0x01ULL << 36)
#define X64_CPUID_ECX                   (0x02ULL << 36)
#define X64_CPUID_EDX                   (0x03ULL << 36)
#define X64_CPUID_REG_MASK              (0x0FULL << 36)

#define X64_HAS_SSE3                    (BIT0_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_PCLMULQDQ               (BIT1_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_DTES64                  (BIT2_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_MONITOR                 (BIT3_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_CPLDEBUGSTORE           (BIT4_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_VMX                     (BIT5_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_SMX                     (BIT6_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_SPEEDSTEP               (BIT7_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_THERMALMONITOR2         (BIT8_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_SSSE3                   (BIT9_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_L1CONTEXTID             (BIT10_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_SILICONDEBUG            (BIT11_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_FMA                     (BIT12_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_CMPXCHG16B              (BIT13_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_TPRUPDATECONTROL        (BIT14_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_PDCM                    (BIT15_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_PCID                    (BIT17_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_DCA                     (BIT18_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_SSE41                   (BIT19_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_SSE42                   (BIT20_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_X2APIC                  (BIT21_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_MOVBE                   (BIT22_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_POPCNT                  (BIT23_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_TSCDEADLINE             (BIT24_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_AESNI                   (BIT25_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_XSAVE                   (BIT26_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_OSXSAVE                 (BIT27_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_AVX                     (BIT28_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_F16C                    (BIT29_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_RDRAND                  (BIT30_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_FPU                     (BIT0_64 | X64_CPUID_0x01 | X64_CPUID_EDX)
#define X64_HAS_V8086                   (BIT1_64 | X64_CPUID_0x01 | X64_CPUID_EDX)
#define X64_HAS_DEBUG                   (BIT2_64 | X64_CPUID_0x01 | X64_CPUID_EDX)
#define X64_HAS_PSE                     (BIT3_64 | X64_CPUID_0x01 | X64_CPUID_EDX)
#define X64_HAS_RDTSC                   (BIT4_64 | X64_CPUID_0x01 | X64_CPUID_EDX)
#define X64_HAS_MSR                     (BIT5_64 | X64_CPUID_0x01 | X64_CPUID_EDX)
#define X64_HAS_PAE                     (BIT6_64 | X64_CPUID_0x01 | X64_CPUID_EDX)
#define X64_HAS_MACH_CHK_EX             (BIT7_64 | X64_CPUID_0x01 | X64_CPUID_EDX)
#define X64_HAS_CMPXCHG8B               (BIT8_64 | X64_CPUID_0x01 | X64_CPUID_EDX)
#define X64_HAS_APIC                    (BIT9_64 | X64_CPUID_0x01 | X64_CPUID_EDX)
#define X64_HAS_SYSENTER                (BIT11_64 | X64_CPUID_0x01 | X64_CPUID_EDX)
#define X64_HAS_MTRR                    (BIT12_64 | X64_CPUID_0x01 | X64_CPUID_EDX)
#define X64_HAS_PGE                     (BIT13_64 | X64_CPUID_0x01 | X64_CPUID_EDX)
#define X64_HAS_MACH_CHK_ARCH           (BIT14_64 | X64_CPUID_0x01 | X64_CPUID_EDX)
#define X64_HAS_CMOV                    (BIT15_64 | X64_CPUID_0x01 | X64_CPUID_EDX)
#define X64_HAS_PAT                     (BIT16_64 | X64_CPUID_0x01 | X64_CPUID_EDX)
#define X64_HAS_PSE36                   (BIT17_64 | X64_CPUID_0x01 | X64_CPUID_EDX)
#define X64_HAS_PSN                     (BIT18_64 | X64_CPUID_0x01 | X64_CPUID_EDX)
#define X64_HAS_CLFLUSH                 (BIT19_64 | X64_CPUID_0x01 | X64_CPUID_EDX)
#define X64_HAS_DEBUGSTORE              (BIT21_64 | X64_CPUID_0x01 | X64_CPUID_EDX)
#define X64_HAS_ACPI                    (BIT22_64 | X64_CPUID_0x01 | X64_CPUID_EDX)
#define X64_HAS_MMX                     (BIT23_64 | X64_CPUID_0x01 | X64_CPUID_EDX)
#define X64_HAS_FXSAVE                  (BIT24_64 | X64_CPUID_0x01 | X64_CPUID_EDX)
#define X64_HAS_SSE                     (BIT25_64 | X64_CPUID_0x01 | X64_CPUID_EDX)
#define X64_HAS_SSE2                    (BIT26_64 | X64_CPUID_0x01 | X64_CPUID_EDX)
#define X64_HAS_SELFSNOOP               (BIT27_64 | X64_CPUID_0x01 | X64_CPUID_EDX)
#define X64_HAS_HYPERTHREADING          (BIT28_64 | X64_CPUID_0x01 | X64_CPUID_EDX)
#define X64_HAS_THERMALMONITOR          (BIT29_64 | X64_CPUID_0x01 | X64_CPUID_EDX)
#define X64_HAS_PBE                     (BIT31_64 | X64_CPUID_0x01 | X64_CPUID_EDX)

// x64 Paging
#define X64_PAGING_TABLE_MAX            0x200ULL
#define X64_PAGING_PRESENT              BIT0_64
#define X64_PAGING_DATA_WRITEABLE       BIT1_64
#define X64_PAGING_CODE_READABLE        BIT1_64
#define X64_PAGING_USER_MODE            BIT2_64
#define X64_PAGING_SUPERVISOR_MODE      0x00ULL
#define X64_PAGING_WRITE_THROUGH        BIT3_64
#define X64_PAGING_CACHE_DISABLE        BIT4_64
#define X64_PAGING_ACCESSED             BIT5_64
#define X64_PAGING_DIRTY                BIT6_64
#define X64_PAGING_IS_PAGES             BIT7_64
#define X64_PAGING_GLOBAL               BIT8_64
#define X64_PAGING_PAT                  BIT12_64
#define X64_PAGING_PTE_PAT              BIT7_64
#define X64_PAGING_NX                   BIT63

// x64 Segments
#define X64_SEG_ACCESSED                BIT40
#define X64_SEG_DATA                    0x00ULL
#define X64_SEG_DATA_WRITEABLE          BIT41
#define X64_SEG_DATA_EXPAND_DOWN        BIT42
#define X64_SEG_CODE                    BIT43
#define X64_SEG_CODE_READABLE           BIT41
#define X64_SEG_CODE_CONFORMING         BIT42
#define X64_SEG_64BIT                   BIT53
#define X64_SEG_DEFAULT32               BIT54
#define X64_SEG_NON_SYSTEM_SEGMENT      BIT44
#define X64_SEG_SYSTEM_SEGMENT          0x00ULL
#define X64_SEG_DPL0                    0x00ULL
#define X64_SEG_DPL1                    BIT45
#define X64_SEG_DPL2                    BIT46
#define X64_SEG_DPL3                    (BIT45 | BIT46)
#define X64_SEG_PRESENT                 BIT47
#define X64_SEG_LIMIT_IN_PAGES          BIT55

#define X64_TYPE_LDT                    0x02
#define X64_TYPE_TSS_AVAILABLE          0x09
#define X64_TYPE_TSS_BUSY_FLAG          BIT1
#define X64_TYPE_CALL_GATE              0x0C
#define X64_TYPE_INT_GATE               0x0E
#define X64_TYPE_TRAP_GATE              0x0F

#define X64_GATE_DPL_MASK               (BIT5 | BIT6)
#define X64_GATE_SEG_PRESENT_BIT        BIT7

#define X64_INT_GATE_PRESENT            0x80
#define X64_4KB_ALIGN_MASK              0x000FFFFFFFFFF000ULL
#define X64_2MB_ALIGN_MASK              0x000FFFFFFFE00000ULL
#define X64_1GB_ALIGN_MASK              0x000FFFFFC0000000ULL

#define X64_PL0                         0x00
#define X64_PL3                         0x03

// PML4 Entry (Top level of 4-level paging
// structure on x64)
// Maps 512GB
typedef UINT64      x64_pml4e;
#define PML4E_FORMAT_PDPT_ADDR(X)       (X & X64_4KB_ALIGN_MASK)

// Page Directory Pointer Table Entry
// Maps 1GB page
typedef UINT64      x64_pdpte;
#define PDPTE_FORMAT_1GB_PAGE_ADDR(X)   (X & X64_1GB_ALIGN_MASK)
#define PDPTE_FORMAT_PAGE_DIR_ADDR(X)   (X & X64_4KB_ALIGN_MASK)

// Page Directory Entry
// Maps 2MB
typedef UINT64      x64_pde;
#define PDE_FORMAT_2MB_PAGE_ADDR(X)     (X & X64_2MB_ALIGN_MASK)
#define PDE_FORMAT_PAGE_TABLE_ADDR(X)   (X & X64_4KB_ALIGN_MASK)

// Page Table Entry
// Maps 4KB
typedef UINT64      x64_pte;
#define PTE_FORMAT_4KB_PAGE_ADDR(X)     (X & X64_4KB_ALIGN_MASK)

// Helpful virtual address macros
#define PML4_INDEX(X)                   (((UINT64)X & 0x7F8000000000ULL) >> 39)
#define PAGE_DIR_PTR_INDEX(X)           (((UINT64)X & 0x7FC0000000ULL) >> 30)
#define PAGE_DIR_INDEX(X)               (((UINT64)X & 0x3FE00000ULL) >> 21)
#define PAGE_TABLE_INDEX(X)             (((UINT64)X & 0x1FF000ULL) >> 12)

// Segment Descriptor
typedef UINT64      x64_seg_descr;
#define SEG_DESCR_FORMAT_LIMIT(X)       ((((UINT64)X & 0x000F0000ULL) << 32) | ((UINT64)X & 0x0000FFFFULL))
#define SEG_DESCR_FORMAT_BASE_ADDR(X)   ((((UINT64)X & 0xFF000000ULL) << 32) | (((UINT64)X & 0x00FF0000ULL) << 16) | (((UINT64)X & 0x0000FFFFULL) << 16))
#define SEG_DESCR_FORMAT_SYSTEM_TYPE(X) (((UINT64)X & 0x000000000000000FULL) << 40)

#define X64_GDT_MAX                     0x100
#define X64_SEGMENT_BASE                0x00ULL
#define X64_SEGMENT_LIMIT               0xFFFFF

#define DPL0_DATA_WRITEABLE             0x100
#define DPL0_CODE_READABLE              0x108
#define DPL0_CODE64_READABLE            0x110

#define DPL3_DATA_WRITEABLE             0x118
#define DPL3_CODE_READABLE              0x120
#define DPL3_CODE64_READABLE            0x128

#define CPU0_TSS_INDEX_GDT              0x130

// Interrupts
#define X64_INTERRUPT_MAX               0x100

#define X64_ADDR_00_15(X)               (X  & 0x000000000000FFFFULL)
#define X64_ADDR_16_31(X)               ((X & 0x00000000FFFF0000ULL) >> 16)
#define X64_ADDR_32_63(X)               (HI32(X))

// Call Gate
typedef PACKED_MS struct s_x64_call_gate {
    UINT16 segment_offset_00_15;
    UINT16 segment_selector;
    UINT8  reserved1;
    UINT8  typeflags;
    UINT16 segment_offset_16_31;
    UINT32 segment_offset_31_63;
    UINT32 high_bits;
} PACKED_GNU x64_call_gate;

// Interrupt Trap Gate
typedef PACKED_MS struct s_x64_inttrap_gate {
    UINT16 procedure_entry_offset_00_15;
    UINT16 segment_selector;
    UINT8  ist;
    UINT8  typeflags;
    UINT16 procedure_entry_offset_16_31;
    UINT32 procedure_entry_offset_32_63;
    UINT32 reserved;
} PACKED_GNU x64_inttrap_gate;

// Task State Segment
typedef PACKED_MS struct s_x64_tss {
    UINT32 reserved1;
    UINT64 rsp[3];
    UINT64 reserved2;
    UINT64 ist[7];
    UINT64 reserved3;
    UINT16 reserved4;
    UINT16 io_map_base_address;
} PACKED_GNU x64_tss;

// cpuinfo results structure
// cpuinfo returns its results in each
// of eax, ebx, ecx & edx depending upon
// the leaf function specified
typedef struct s_x64_cpuinfo_results {
    UINT32 reg[4];
} x64_cpuinfo_results;

// x64 cpu structure
// cpuinfo [0..2] is for cpuid
// cpuinfo [3..10] is for cpuidex
typedef struct s_x64_cpu {
    UINT32 max_cpuid_eax;
    UINT32 max_cpuidex_eax;
    UINT16 physical_address_bits;
    UINT16 linear_address_bits;
    x64_cpuinfo_results cpuinfo[11];
} x64_cpu;

// x64 80-bit far pointer
typedef PACKED_MS struct s_x64_farptr {
    UINT16 segment_selector;
    UINT64 base_address;
} PACKED_GNU x64_farptr;

// x64 segment selector
typedef PACKED_MS struct s_x64_seg_sel {
    UINT16 limit;
    UINT64 base;
} PACKED_GNU x64_seg_sel;

// This 4KB structure represents a virtual address space mapping
typedef PACKED_MS struct s_x64_virtual_address_space {
    x64_pml4e *pml4;
    x64_pdpte *pdpt;
    x64_pde   *pde;
    x64_pte   *pte;
} PACKED_GNU x64_virtual_address_space;

// Structure for facilitating
// one-time kernel allocations in preboot
typedef struct s_x64_preboot_mem_block {
    VOID *base_addr;
    UINTN size;
    VOID *current_addr;
    UINTN free_space;
    UINT8 alignment;
    UINTN wasted_space;
} x64_preboot_mem_block;

// This structure represents a kernel stack
typedef struct s_x64_kernel_stack {
    UINT64 guard_page_low_base;
    UINT64 stack_base;
    UINT64 guard_page_high_base;
} x64_kernel_stack;

// cpu struct
extern x64_cpu cpu;

// for paging structure masks
#define MAXPHYADDR  cpu.physical_address_bits

// Function prototypes
nebStatus x64ClearVirtualAddressSpace(x64_virtual_address_space *vas);
VOID x64InitBootCPU();
BOOLEAN x64ReadCpuinfoFlags(UINT64 flag);
VOID x64InitGDT();
VOID x64InitKernelStacks();
VOID x64InitIDT();
EFI_VIRTUAL_ADDRESS x64GetCurrentPML4TableAddr();
VOID* x64AllocateRandomMemory(x64_preboot_mem_block *mb, UINT64 size, UINT64 alignment_mask);
VOID x64AllocateBootScratchArea();
VOID x64BuildInitialKernelPageTable();
VOID x64DumpGdt();
VOID x64AllocateSystemStruct();
UINT64* x64GetPageInfo(EFI_VIRTUAL_ADDRESS addr);

// Functions defined in assembler
extern VOID EFIAPI x64EnableInterrupts();
extern VOID EFIAPI x64DisableInterrupts();
extern VOID EFIAPI x64OutportB(IN UINT16 Port, OUT UINT8 Value);
extern VOID EFIAPI x64OutportW(IN UINT16 Port, OUT UINT16 Value);
extern UINT8 EFIAPI x64InportB(IN UINT16 Port);
extern UINT16 EFIAPI x64InportW(IN UINT16 Port);
extern UINT64 EFIAPI x64ReadTsc();
extern x64_farptr* EFIAPI x64LoadStackSegmentAndJump(IN x64_farptr* ssptr, IN UINT64* dest);
extern UINTN EFIAPI x64WriteCR3(IN UINTN new_cr3);
extern VOID EFIAPI x64ReadGdtr(OUT x64_seg_sel  *gdtr);
extern VOID EFIAPI x64WriteGdtr(IN CONST x64_seg_sel *gdtr);
extern VOID EFIAPI x64ReadIdtr(OUT x64_seg_sel  *idtr);
extern VOID EFIAPI x64WriteIdtr(OUT x64_seg_sel  *idtr);
extern UINT64 EFIAPI x64ReadCR2();
extern UINT16 EFIAPI x64ReadCS();
extern UINT16 EFIAPI x64ReadDS();
extern UINT16 EFIAPI x64ReadSS();
extern UINT16 EFIAPI x64ReadES();
extern UINT16 EFIAPI x64ReadFS();
extern UINT16 EFIAPI x64ReadGS();
extern VOID EFIAPI x64LoadTR(UINT16 tr_gdt_descr_index);

#endif /* __K0_X64_H */
