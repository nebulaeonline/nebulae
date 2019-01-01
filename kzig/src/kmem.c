// Copyright (c) 2018 Nebulae Foundation. All rights reserved.
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

//
// Basic UEFI Libraries
//
#include <Library/UefiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>

//
// Boot and Runtime Services
//
#include <Library/UefiBootServicesTableLib.h>

// 
// Kernel Headers
//
#include "include/k.h"
#include "include/kmem.h"
#include "include/isaac64.h"

// Memory Type Strings
const CHAR16 *EFI_MEMORY_TYPES[] = {
    L"Reserved",
    L"Loader Code",
    L"Loader Data",
    L"Boot Services Code",
    L"Boot Services Data",
    L"Runtime Services Code",
    L"Runtime Services Data",
    L"Conventional Memory",
    L"Unusable Memory",
    L"ACPI Reclaim Memory",
    L"ACPI Memory NVS",
    L"Memory Mapped I/O",
    L"Memory Mapped I/O Port Space",
    L"Pal Code",
    L"Persistent Memory"
};

#define EFI_MEMORY_RESERVED             0x00
#define EFI_MEMORY_LOADERCODE           0x01
#define EFI_MEMORY_LOADERDATA           0x02
#define EFI_MEMORY_BSCODE               0x03
#define EFI_MEMORY_BSDATA               0x04
#define EFI_MEMORY_RSCODE               0x05
#define EFI_MEMORY_RSDATA               0x06
#define EFI_MEMORY_CONVENTIONAL         0x07
#define EFI_MEMORY_UNUSUABLE            0x08
#define EFI_MEMORY_ACPIRECLAIM          0x09
#define EFI_MEMORY_ACPINVS              0x0A
#define EFI_MEMORY_MMIO                 0x0B
#define EFI_MEMORY_MMIOPORT             0x0C
#define EFI_MEMORY_PAL                  0x0D
#define EFI_MEMORY_PERSISTENT           0x0E

//
// Memory Subsystem Vars 
//
UINTN kmem_conventional_pages = 0;
VOID* kmem_largest_block = NULL;
UINTN kmem_largest_block_size = 0;
UINTN kmem_largest_block_page_count = 0;

//
// Initialize the memory subsystem by performing the following tasks:
// 
//  1) Read UEFI memmap to obtain size of memory map
//  2) Allocate a temporary buffer large enough to hold the memory map
//  3) Calculate the number of free conventional memory pages, along
//      with the size and location of the largest block of free
//      conventional memory
//  4) Initialize the CSPRNG
//  5) Allocate enough pages for the memory map and the kernel page 
//      stack at a random address in the largest contiguous block of
//      conventional memory
void InitMemSubsystem() {
    
    EFI_STATUS exit_status = EFI_SUCCESS;
    
    UINTN memmap_pages_to_alloc = 0;
    VOID *memmap_ptr = NULL;
    
    // Vars required for BootServices->GetMemoryMap call
    EFI_STATUS memmap_status;
    UINTN memmap_size = 0;
    UINTN memmap_key = 0;
    UINTN memmap_descriptor_size = 0;
    UINT32 memmap_descriptor_version = 0;
    
    // Call GetMemoryMap first with a NULL pointer to get the (rough) size of the memory map
    memmap_status = gBS->GetMemoryMap(&memmap_size, memmap_ptr, &memmap_key, &memmap_descriptor_size, &memmap_descriptor_version);
    if (EFI_ERROR(memmap_status) && memmap_status != EFI_BUFFER_TOO_SMALL) {
        Print(L"Unable to query UEFI for Memory Map size: %r\n", memmap_status);
        exit_status = memmap_status;
        goto exit;
    }

    if (kDBG)
        Print(L"Map Size: %lu, Map Key: %lu, Desc. Size: %lu, Desc. Version: %u\n",
            memmap_size, memmap_key, memmap_descriptor_size, memmap_descriptor_version);

    // Allocate enough memory to hold a temporary copy of the memory map
    memmap_pages_to_alloc = (memmap_size / UEFI_PAGE_SIZE) + 1;
    memmap_size = memmap_pages_to_alloc * UEFI_PAGE_SIZE;
    memmap_ptr = AllocatePages(memmap_pages_to_alloc);
    if (memmap_ptr == NULL) {
        Print(L"Unable to allocate pages for Memory Map\n");
        exit_status = EFI_BAD_BUFFER_SIZE;
        goto exit;
    }

    if (kDBG)
        Print(L"Allocated %u page(s)\n", memmap_pages_to_alloc);

    // Call GetMemoryMap again to get the actual memory map
    memmap_status = gBS->GetMemoryMap(&memmap_size, memmap_ptr, &memmap_key, &memmap_descriptor_size, &memmap_descriptor_version);
    if (EFI_ERROR(memmap_status)) {
        Print(L"Unable to query UEFI for Memory Map: %r\n", memmap_status);
        exit_status = memmap_status;
        goto exit;
    }

    // Parse the UEFI memory map in order to count the available
    // conventional memory
    EFI_MEMORY_DESCRIPTOR *memmap_entry = NULL;
    UINT64 memmap_entries = memmap_size / memmap_descriptor_size;
    VOID *memmap_iter = memmap_ptr;

    if (kDBG)
        Print(L"Map Size: %lu, Map Key: %lu, Desc. Size: %lu, Desc. Version: %u\n",
            memmap_size, memmap_key, memmap_descriptor_size, memmap_descriptor_version);

    for (UINTN i = 0; i < memmap_entries; i++) {
        memmap_entry = (EFI_MEMORY_DESCRIPTOR *)memmap_iter;
        if (memmap_entry->Type == EFI_MEMORY_CONVENTIONAL) {
            kmem_conventional_pages += memmap_entry->NumberOfPages;
            if (memmap_entry->NumberOfPages > kmem_largest_block_page_count) {
                kmem_largest_block_page_count = memmap_entry->NumberOfPages;
                kmem_largest_block_size = kmem_largest_block_page_count * UEFI_PAGE_SIZE;
                kmem_largest_block = (VOID*)memmap_entry->PhysicalStart;
            }
        }

        if (kDBG)
            Print(L"T: %s, P: %lu, V: %lu, #: %lu, A: %lx\n",
                EFI_MEMORY_TYPES[memmap_entry->Type], memmap_entry->PhysicalStart, memmap_entry->VirtualStart,
                memmap_entry->NumberOfPages, memmap_entry->Attribute);
        memmap_iter = (CHAR8*)memmap_iter + memmap_descriptor_size;
    }

    // Initialize the isaac64 csprng
    randinit(TRUE);

    /* GetCSPRNG64((UINT64)kmem_largest_block, (UINT64)kmem_largest_block + kmem_largest_block_size) */

exit:
    FreePages(memmap_ptr, memmap_pages_to_alloc);
    if (kDBG) {
        Print(L"Freed %u page(s)\n", memmap_pages_to_alloc);
        Print(L"Pages of Conventional Memory: %lu\n", kmem_conventional_pages);
        Print(L"Largest Block: %lu\n", kmem_largest_block);
        Print(L"Largest Block Size: %lu\n", kmem_largest_block_size);
        Print(L"Largest Block Page Count: %lu\n", kmem_largest_block_page_count);            
    }
}
