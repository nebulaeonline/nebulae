// Copyright (c) 2005-2019 Nebulae Foundation. All rights reserved.
// Portions Copyright(c) 2003 - 2005, Robert C.Horvath, III
// Portions Copyright (c) 2006 - 2017, Intel Corporation. All rights reserved.
// Portions Copyright(c) 2008-2009, Apple Inc. All rights reserved.
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

#ifndef __K0_K0_H
#define __K0_K0_H

#define NEBULAE_SIG                     0x0065616C7562656EULL
#define NEBULAE_VERSION_MAJOR           0x01
#define NEBULAE_VERSION_MINOR           0x01
#define NEBULAE_VERSION_BUILD           0x35

// Structure packing
#ifdef _MSC_VER
#define PACKED_MS                       _declspec(align(1))
#define PACKED_GNU
#elif defined(__clang__) || defined(__GNUC__)
#define PACKED_MS
#define PACKED_GNU                      __attribute__((packed))
#endif

// Helpful constants
#define HI32_MASK                       0xFFFFFFFF00000000ULL
#define LO32_MASK                       0x00000000FFFFFFFFULL

#define PAGE_SIZE_4KB                   SIZE_4KB
#define PAGE_SIZE_2MB                   SIZE_2MB
#define PAGE_SIZE_1GB                   SIZE_1GB

#define ALIGN_8                         0x08
#define ALIGN_16                        0x10
#define ALIGN_32                        0x20
#define ALIGN_64                        0x40
#define ALIGN_128                       0x80
#define ALIGN_4KB                       0x1000
#define ALIGN_2MB                       0x200000

// Helpful macros
#define CHECK_BIT(INPUT, MASK)          ((INPUT & MASK) == MASK)
#define HI32(X)                         (UINT32)((((UINT64)(X)) & HI32_MASK) >> 32)
#define LO32(X)                         (UINT32)(((UINT64)(X)) & LO32_MASK)
#define ISNULL(X)                       (X == NULL)
#define UDIV_UP(A, B)                   ((((UINT64)A) + ((UINT64)B) - 1ULL) / ((UINT64)B))
#define ALIGN_UP(A, B)                  (UDIV_UP((UINT64)A, (UINT64)B) * ((UINT64)B))

#ifdef __GNUC__
#if __GNUC__ >= 4
#define OFFSET_OF(TYPE, Field) ((UINTN) __builtin_offsetof(TYPE, Field))
#endif
#endif

#ifndef OFFSET_OF
#define OFFSET_OF(TYPE, Field) ((UINTN) &(((TYPE *)0)->Field))
#endif

// 128-bit wide unsigned integer data type
typedef PACKED_MS struct s_x64_u128 {
    UINT64 lo64;
    UINT64 hi64;
} PACKED_GNU x64_u128;

// Debug mode(s)
extern BOOLEAN k0_VERBOSE_DEBUG;
extern BOOLEAN k0_PRECONFIG_DEBUG;
extern BOOLEAN k0_PAGETABLE_DEBUG;

// How much memory to allocate for the system table
#define NEBULAE_SYSTEM_TABLE_RESERVED_BYTES             SIZE_8KB

// Have we called our actual kernel entry function yet?
extern BOOLEAN k0_main_called;

typedef PACKED_MS struct s_nebulae_system_table {
    UINT64  magic;
    UINT8   version_major;
    UINT8   version_minor;
    UINT32  version_build;
    UINT16  data_offset;
    UINT64  free_space;
    UINT64  *next_free_byte;
    EFI_HANDLE  uefi_image_handle;
    EFI_SYSTEM_TABLE *uefi_system_table;
} PACKED_GNU nebulae_system_table;

// Function signatures
NORETURN VOID k0_main();
NORETURN VOID kernel_panic(IN CONST CHAR16  *Format, ...);

// Error type
#include "../include/klib/kerror.h"

// We always want issac64 a step away
#include "../deps/isaac64.h"

// jsmn too
#include "../deps/jsmn.h"

#endif /* __K0_K0_H */
