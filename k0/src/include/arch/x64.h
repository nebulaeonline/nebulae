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
// 3. Neither the name of the copyright holder nor the names of its contributors 
// may be used to endorse or promote products derived from this software without 
// specific prior written permission.
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

#ifndef X64_H
#define X64_H

#include <Library/UefiLib.h>

// Structure packing
#ifdef _MSC_VER
#define PACKED_MS     _declspec(align(1))
#define PACKED_GNU
#elif defined(__clang__) || defined(__GNUC__)
#define PACKED_MS
#define PACKED_GNU    __attribute__((packed))
#endif

// Helpful constants
#define X64_REG_EAX                 0x00ULL
#define X64_REG_EBX                 0x01ULL
#define X64_REG_ECX                 0x02ULL
#define X64_REG_EDX                 0x03ULL

#define BIT0_64                     0x01ULL
#define BIT1_64                     (0x01ULL << 1)
#define BIT2_64                     (0x01ULL << 2)
#define BIT3_64                     (0x01ULL << 3)
#define BIT4_64                     (0x01ULL << 4)
#define BIT5_64                     (0x01ULL << 5)
#define BIT6_64                     (0x01ULL << 6)
#define BIT7_64                     (0x01ULL << 7)
#define BIT8_64                     (0x01ULL << 8)
#define BIT9_64                     (0x01ULL << 9)
#define BIT10_64                    (0x01ULL << 10)
#define BIT11_64                    (0x01ULL << 11)
#define BIT12_64                    (0x01ULL << 12)
#define BIT13_64                    (0x01ULL << 13)
#define BIT14_64                    (0x01ULL << 14)
#define BIT15_64                    (0x01ULL << 15)
#define BIT16_64                    (0x01ULL << 16)
#define BIT17_64                    (0x01ULL << 17)
#define BIT18_64                    (0x01ULL << 18)
#define BIT19_64                    (0x01ULL << 19)
#define BIT20_64                    (0x01ULL << 20)
#define BIT21_64                    (0x01ULL << 21)
#define BIT22_64                    (0x01ULL << 22)
#define BIT23_64                    (0x01ULL << 23)
#define BIT24_64                    (0x01ULL << 24)
#define BIT25_64                    (0x01ULL << 25)
#define BIT26_64                    (0x01ULL << 26)
#define BIT27_64                    (0x01ULL << 27)
#define BIT28_64                    (0x01ULL << 28)
#define BIT29_64                    (0x01ULL << 29)
#define BIT30_64                    (0x01ULL << 20)
#define BIT31_64                    (0x01ULL << 31)

// CPU feature flags
#define X64_CPUID_0x00              0x00ULL
#define X64_CPUID_0x01              (0x01ULL << 32)
#define X64_CPUID_0x02              (0x02ULL << 32)
#define X64_CPUID_0x03              (0x03ULL << 32)
#define X64_CPUID_0x04              (0x04ULL << 32)
#define X64_CPUID_0x05              (0x05ULL << 32)
#define X64_CPUID_0x06              (0x06ULL << 32)
#define X64_CPUID_0x07              (0x07ULL << 32)
#define X64_CPUID_0x08              (0x08ULL << 32)
#define X64_CPUID_0x09              (0x09ULL << 32)
#define X64_CPUID_0x0A              (0x0AULL << 32)
#define X64_CPUID_MASK              (0x0FULL << 32)

#define X64_CPUID_EAX               0x00ULL
#define X64_CPUID_EBX               (0x01ULL << 36)
#define X64_CPUID_ECX               (0x02ULL << 36)
#define X64_CPUID_EDX               (0x03ULL << 36)
#define X64_CPUID_REG_MASK          (0x0FULL << 36)

#define X64_HAS_SSE3                (BIT0_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_PCLMULQDQ           (BIT1_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_DTES64              (BIT2_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_MONITOR             (BIT3_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_CPLDEBUGSTORE       (BIT4_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_VMX                 (BIT5_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_SMX                 (BIT6_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_SPEEDSTEP           (BIT7_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_THERMALMONITOR2     (BIT8_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_SSSE3               (BIT9_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_L1CONTEXTID         (BIT10_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_SILICONDEBUG        (BIT11_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_FMA                 (BIT12_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_CMPXCHG16B          (BIT13_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_TPRUPDATECONTROL    (BIT14_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_PDCM                (BIT15_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_PCID                (BIT17_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_DCA                 (BIT18_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_SSE41               (BIT19_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_SSE42               (BIT20_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_X2APIC              (BIT21_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_MOVBE               (BIT22_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_POPCNT              (BIT23_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_TSCDEADLINE         (BIT24_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_AESNI               (BIT25_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_XSAVE               (BIT26_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_OSXSAVE             (BIT27_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_AVX                 (BIT28_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_F16C                (BIT29_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_RDRAND              (BIT30_64 | X64_CPUID_0x01 | X64_CPUID_ECX)
#define X64_HAS_FPU                 (BIT0_64 | X64_CPUID_0x01 | X64_CPUID_EDX)
#define X64_HAS_V8086               (BIT1_64 | X64_CPUID_0x01 | X64_CPUID_EDX)
#define X64_HAS_DEBUG               (BIT2_64 | X64_CPUID_0x01 | X64_CPUID_EDX)
#define X64_HAS_PSE                 (BIT3_64 | X64_CPUID_0x01 | X64_CPUID_EDX)
#define X64_HAS_RDTSC               (BIT4_64 | X64_CPUID_0x01 | X64_CPUID_EDX)
#define X64_HAS_MSR                 (BIT5_64 | X64_CPUID_0x01 | X64_CPUID_EDX)
#define X64_HAS_PAE                 (BIT6_64 | X64_CPUID_0x01 | X64_CPUID_EDX)
#define X64_HAS_MACHINECHECKEXCEPTION (BIT7_64 | X64_CPUID_0x01 | X64_CPUID_EDX)
#define X64_HAS_CMPXCHG8B           (BIT8_64 | X64_CPUID_0x01 | X64_CPUID_EDX)
#define X64_HAS_APIC                (BIT9_64 | X64_CPUID_0x01 | X64_CPUID_EDX)
#define X64_HAS_SYSENTER            (BIT11_64 | X64_CPUID_0x01 | X64_CPUID_EDX)
#define X64_HAS_MTRR                (BIT12_64 | X64_CPUID_0x01 | X64_CPUID_EDX)
#define X64_HAS_PGE                 (BIT13_64 | X64_CPUID_0x01 | X64_CPUID_EDX)
#define X64_HAS_MACHINECHECKARCHITECTURE (BIT14_64 | X64_CPUID_0x01 | X64_CPUID_EDX)
#define X64_HAS_CMOV                (BIT15_64 | X64_CPUID_0x01 | X64_CPUID_EDX)
#define X64_HAS_PAT                 (BIT16_64 | X64_CPUID_0x01 | X64_CPUID_EDX)
#define X64_HAS_PSE36               (BIT17_64 | X64_CPUID_0x01 | X64_CPUID_EDX)
#define X64_HAS_PSN                 (BIT18_64 | X64_CPUID_0x01 | X64_CPUID_EDX)
#define X64_HAS_CLFLUSH             (BIT19_64 | X64_CPUID_0x01 | X64_CPUID_EDX)
#define X64_HAS_DEBUGSTORE          (BIT21_64 | X64_CPUID_0x01 | X64_CPUID_EDX)
#define X64_HAS_ACPI                (BIT22_64 | X64_CPUID_0x01 | X64_CPUID_EDX)
#define X64_HAS_MMX                 (BIT23_64 | X64_CPUID_0x01 | X64_CPUID_EDX)
#define X64_HAS_FXSAVE              (BIT24_64 | X64_CPUID_0x01 | X64_CPUID_EDX)
#define X64_HAS_SSE                 (BIT25_64 | X64_CPUID_0x01 | X64_CPUID_EDX)
#define X64_HAS_SSE2                (BIT26_64 | X64_CPUID_0x01 | X64_CPUID_EDX)
#define X64_HAS_SELFSNOOP           (BIT27_64 | X64_CPUID_0x01 | X64_CPUID_EDX)
#define X64_HAS_HYPERTHREADING      (BIT28_64 | X64_CPUID_0x01 | X64_CPUID_EDX)
#define X64_HAS_THERMALMONITOR      (BIT29_64 | X64_CPUID_0x01 | X64_CPUID_EDX)
#define X64_HAS_PBE                 (BIT31_64 | X64_CPUID_0x01 | X64_CPUID_EDX)

// x64 Paging
#define X64_PAGING_PRESENT            BIT0_64
#define X64_PAGING_WRITEABLE          BIT1_64
#define X64_PAGING_USER_MODE          BIT2_64
#define X64_PAGING_WRITE_THROUGH      BIT3_64
#define X64_PAGING_CACHE_DISABLE      BIT4_64
#define X64_PAGING_ACCESSED           BIT5_64
#define X64_PAGING_DIRTY              BIT6_64
#define X64_PAGING_IS_PAGES           BIT7_64
#define X64_PAGING_GLOBAL             BIT8_64
#define X64_PAGING_PAT                BIT12_64
#define X64_PAGING_PTE_PAT            BIT7_64
#define X64_PAGING_NX                 BIT63

#define X64_SEG_ACCESSED            BIT40
#define X64_SEG_DATA                0x00ULL
#define X64_SEG_DATA_WRITEABLE      BIT41
#define X64_SEG_DATA_EXPAND_DOWN    BIT42
#define X64_SEG_DATA_BIG            BIT54
#define X64_SEG_CODE                BIT43
#define X64_SEG_CODE_READABLE       BIT41
#define X64_SEG_CODE_CONFORMING     BIT42
#define X64_SEG_CODE_64BIT          BIT53
#define X64_SEG_CODE_DEFAULT32      BIT54
#define X64_SEG_NON_SYSTEM_SEGMENT  BIT44
#define X64_SEG_DPL0                0x00ULL
#define X64_SEG_DPL1                BIT45
#define X64_SEG_DPL2                BIT46
#define X64_SEG_DPL3                (BIT45 | BIT46)
#define X64_SEG_PRESENT             BIT47
#define X64_SEG_LIMIT_IN_PAGES      BIT55

#define X64_SEG_SYS_LDT             (0x02ULL << 40)
#define X64_SEG_SYS_TSS_AVAILABLE   (0x09ULL << 40)
#define X64_SEG_SYS_TSS_BUSY        (0x0BULL << 40)
#define X64_SEG_SYS_CALL_GATE       (0x0CULL << 40)
#define X64_SEG_SYS_INT_GATE        (0x0EULL << 40)
#define X64_SEG_SYS_TRAP_GATE       (0x0FULL << 40)

// PML4 Entry (Top level of 4-level paging
// structure on x64)
// Maps 512GB
typedef UINT64      x64_pml4e;
#define PML4E_FORMAT_PDPT_ADDR(X)       (X & 0x000FFFFFFFFFF000ULL)

// Page Directory Pointer Table Entry
// Maps 1GB page
typedef UINT64      x64_pdpte;
#define PDPTE_FORMAT_1GB_PAGE_ADDR(X)   (X & 0x000FFFFFE0000000ULL)
#define PDPTE_FORMAT_PAGE_DIR_ADDR(X)   (X & 0x000FFFFFFFFFF000ULL)

// Page Directory Entry
// Maps 2MB
typedef UINT64      x64_pde;
#define PDE_FORMAT_2MB_PAGE_ADDR(X)     (X & 0x000FFFFFFFF00000ULL)
#define PDE_FORMAT_PAGE_TABLE_ADDR(X)   (X & 0x000FFFFFFFFFF000ULL)

// Page Table Entry
// Maps 4KB
typedef UINT64      x64_pte;
#define PTE_FORMAT_4KB_PAGE_ADDR(X)     (X & 0x000FFFFFFFFFF000ULL)

// Segment Descriptor
typedef UINT64      x64_seg_descr;
#define SEG_DESCR_FORMAT_LIMIT(X)       ((((UINT64)X & 0x000F0000ULL) << 32) | ((UINT64)X & 0x0000FFFFULL))
#define SEG_DESCR_FORMAT_BASE_ADDR(X)   ((((UINT64)X & 0xFF000000ULL) << 32) | ((UINT64)X & 0x00FF0000ULL << 16) | (((UINT64)X & 0x0000FFFFULL) << 16))

// Call Gate
typedef PACKED_MS struct s_x64_call_gate {
    UINT64 low;
    UINT64 high;
} PACKED_GNU x64_call_gate;

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
    x64_cpuinfo_results cpuinfo[11];
} x64_cpu;

// cpu struct
extern x64_cpu cpu;

// Function prototypes
void InitArchCPU(void);
BOOLEAN ReadCpuinfoFlag(UINT64 flag);

#endif /* X64_H */
