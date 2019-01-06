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

#ifdef __NEBULAE_ARCH_X64
#include <Library/BaseLib.h>
#include <Library/UefiLib.h>

#include "include/k0.h"
#include "include/arch/x64/x64.h"

x64_cpu cpu;

void InitArchCPU() {

    // First call cpuid with eax == 0x00
    // that call returns both the maximum
    // supported eax value in eax, along 
    // with the processor vendor string
    // in EBX, ECX & EDX
    EFI_STATUS cpuid_result = EFI_SUCCESS;

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

    UINT32 current_cpuidex_fn;
    for (current_cpuidex_fn = 0x80000001; current_cpuidex_fn <= cpu.max_cpuidex_eax; ++current_cpuidex_fn) {
        
        cpuid_result = AsmCpuid(current_cpuidex_fn,
            &cpu.cpuinfo[current_cpuidex_fn - 0x7FFFFFFD].reg[X64_REG_EAX],
            &cpu.cpuinfo[current_cpuidex_fn - 0x7FFFFFFD].reg[X64_REG_EBX],
            &cpu.cpuinfo[current_cpuidex_fn - 0x7FFFFFFD].reg[X64_REG_ECX],
            &cpu.cpuinfo[current_cpuidex_fn - 0x7FFFFFFD].reg[X64_REG_EDX]);

        if (EFI_ERROR(cpuid_result)) {
            kernel_panic("Unable to query the cpu (eax == 0x%x): %r\n", current_cpuidex_fn, cpuid_result);
        }
    }
}

// Function to parse the cpuinfo structs for 
// processor feature information
BOOLEAN ReadCpuinfoFlag(UINT64 flag) {
    UINT32 bit, cpuid, reg;

    bit = (UINT32)(flag & 0x00000000FFFFFFFFULL);
    cpuid = ((flag & X64_CPUID_MASK) >> 32);
    reg = ((flag & X64_CPUID_REG_MASK) >> 36);

    return CHECK_BIT(cpu.cpuinfo[cpuid].reg[reg], bit);
}

#endif
