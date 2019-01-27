// Copyright (c) 2005-2019 Nebulae Foundation. All rights reserved.
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

#define preboot_mem_block           x64_preboot_mem_block

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

preboot_mem_block* InitPrebootMemBlock(preboot_mem_block *pbmb, CHAR8 id[5], VOID *base_addr, UINTN block_size);
VOID* kPrebootMalloc(preboot_mem_block *pbmb, UINTN allocation_size, UINT64 desired_alignment);
VOID* kPrebootCriticalMalloc(preboot_mem_block *pbmb, UINTN allocation_size, UINT64 desired_alignment);

EFI_PHYSICAL_ADDRESS* AllocPhysicalPage(UINTN page_size);
EFI_PHYSICAL_ADDRESS* AllocPageContainingAddr(EFI_PHYSICAL_ADDRESS *addr, OUT UINTN *page_size);

nebStatus FreePhysicalPage(EFI_PHYSICAL_ADDRESS *base_addr, UINTN page_size);
UINT64    GetFreeMemStackCount(UINT32 which_size);
UINT64    GetAllocatedMemStackCount(UINT32 which_size);
nebStatus InitMem();
UINTN     ReadUefiMemoryMap();
VOID      AllocateSystemStruct();
nebStatus RemoveFreePageContainingAddr(UINT64 addr);
BOOLEAN   IsPageFree_Preboot(UINT64 addr);
UINT64*   GetPageInfo(EFI_VIRTUAL_ADDRESS addr);

#endif /* __K0_KMEM_H */
