// Copyright (c) 2018-2019 Nebulae Foundation. All rights reserved.
// Copyright(c) 2017, Intel Corporation. All rights reserved.
// Copyright(c) 2017, AMD Incorporated. All rights reserved.
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

#ifndef __K0_KMEM_H
#define __K0_KMEM_H

#ifdef __NEBULAE_ARCH_X64
#include "../x64/x64.h"

#define ALIGN_MASK_4KB              X64_4KB_ALIGN_MASK
#define ALIGN_MASK_2MB              X64_2MB_ALIGN_MASK
#define ALIGN_MASK_1GB              X64_1GB_ALIGN_MASK

#define PAGE_4KB_SUPERVISOR         0x0000000000000FFFULL
#define PAGE_4KB_USER               0x0000000000000EEEULL
#define PAGE_2MB_SUPERVISOR         0x00000000000FFFFFULL
#define PAGE_2MB_USER               0x00000000000EEEEEULL

#endif

// Structure to hold info about
// the uefi memory map
typedef struct s_uefi_memory_map_info {
    EFI_MEMORY_DESCRIPTOR* memory_map;
    UINTN page_count;
    UINTN size;
    UINTN key;
    UINTN descr_size;
    UINT32 descr_version;
} uefi_memory_map_info;

// Our memory map
uefi_memory_map_info memmap;

UINT64    GetFreeMemStackCount(UINT32 which_size);
UINT64    GetAllocatedMemStackCount(UINT32 which_size);
nebStatus InitMem(VOID);
UINTN     ReadUefiMemoryMap(VOID);
VOID      AllocateSystemStruct(VOID);
nebStatus RemovePageContainingAddr(UINT64 addr);
UINT64*   GetPageInfo(EFI_VIRTUAL_ADDRESS addr);
UINT64    GetPage(UINTN page_size);

#endif /* __K0_KMEM_H */
