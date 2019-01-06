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

// UEFI Debug Library (ASSERT) & Boot Services
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>

// Kernel includes
#include "include/k0.h"
#include "include/kmem.h"
#include "include/isaac64.h"

#include "include/arch/x64.h"

// Kernel Entrypoint
NORETURN void k0_main(void) {
    
    // Initialize the Isaac64 CSPRNG
    // Isaac64 generates a 64-bit cryptographically
    // secure pseudo-random number in 19 cycles 
    // on x64
    InitIsaac64CSPRNG(TRUE);

    // Initialize the cpu architecture
    InitArchCPU();

    // Initialize the memory subsystem
    InitMemSubsystem();

    // Do something
    if (k0_VERBOSE_DEBUG) {
        Print(L"Cpuid (0x01) ecx == 0x%x / edx == 0x%x\n", cpu.cpuinfo[1].reg[X64_REG_ECX], cpu.cpuinfo[1].reg[X64_REG_EDX]);
        Print(L"X64_SEG_SYS_CALL_GATE == %lx\n", X64_SEG_SYS_CALL_GATE);
    }

    // Shutdown the memory subsystem
    ShutdownMemSubsystem();

    // Woo-hoo!
    while (TRUE) {}
}

// Panic function
NORETURN void kernel_panic(IN CONST CHAR16  *Format, ...) {
    VA_LIST Marker;

    VA_START(Marker, Format);

    InternalPrint(Format, gST->ConOut, Marker);

    VA_END(Marker);
    
    while (TRUE) {}
}
