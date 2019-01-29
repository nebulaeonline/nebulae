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

#ifndef __K0_INTERRUPT_H
#define __K0_INTERRUPT_H

typedef VOID(*IntHandler)();

#ifdef __NEBULAE_ARCH_X64
#include "../arch/x64/acpi.h"
#define INTERRUPT_VECTOR_COUNT      0xFF

typedef PACKED_MS struct s_nebulae_interrupt {
    IntHandler isr_fn;
    UINT32 data;
    UINT32 status;
    UINT32 flags;
    BOOLEAN present;
} PACKED_GNU nebulae_interrupt;
#endif

VOID RegisterInterruptHandler(UINT64 vector, nebulae_interrupt interrupt);
VOID UnRegisterInterruptHandler(UINT64 vector);
VOID ExceptionHandler(UINT64 vector, UINT64 error_code);
VOID IsrHandler(unsigned int vector);
VOID NMIHandler();

#endif // __K0_INTERRUPT_H
