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

#ifndef __K0_K0_H
#define __K0_K0_H

// Structure packing
#ifdef _MSC_VER
#define PACKED_MS     _declspec(align(1))
#define PACKED_GNU
#elif defined(__clang__) || defined(__GNUC__)
#define PACKED_MS
#define PACKED_GNU    __attribute__((packed))
#endif

// Helpful constants
#define HI32_MASK                       0xFFFFFFFF00000000
#define LO32_MASK                       0x00000000FFFFFFFF

#define PAGE_SIZE_4KB                   SIZE_4KB
#define PAGE_SIZE_2MB                   SIZE_2MB
#define PAGE_SIZE_1GB                   SIZE_1GB

// Helpful macros
#define CHECK_BIT(INPUT, MASK)          ((INPUT & MASK) == MASK)
#define HI32(X)                         (UINT32)((((UINT64)(X)) & HI32_MASK) >> 32)
#define LO32(X)                         (UINT32)(((UINT64)(X)) & LO32_MASK)

// 128-bit wide unsigned integer data type
typedef PACKED_MS struct s_x64_u128 {
    UINT64 lo;
    UINT64 hi;
} PACKED_GNU x64_u128;

// Debug mode(s)
extern BOOLEAN k0_VERBOSE_DEBUG;
extern BOOLEAN k0_PRECONFIG_DEBUG;

// Handle to the kernel image 
extern EFI_HANDLE nebulae_uefi_image_handle;

// Pointer to the UEFI system table
extern EFI_SYSTEM_TABLE* nebulae_uefi_system_table;

// Function signatures
NORETURN VOID k0_main(VOID);
NORETURN VOID kernel_panic(IN CONST CHAR16  *Format, ...);

// We always want issac64 a step away
#include "../deps/isaac64.h"

#endif /* __K0_K0_H */
