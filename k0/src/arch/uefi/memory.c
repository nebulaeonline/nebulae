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

// Basic UEFI Libraries
#include <Library/UefiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>

// Boot and Runtime Services
#include <Library/UefiBootServicesTableLib.h>

// Kernel Headers
#include "../../include/k0.h"
#include "../../include/klib/kstack.h"
#include "../../include/klib/kerror.h"

// Uefi Memory Header
#include "../../include/arch/uefi/memory.h"

// Architecure Specific Headers
#ifdef __NEBULAE_ARCH_X64
#include "../../include/arch/x64/x64.h"
#endif

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

// Memory Subsystem Vars 
UINTN kmem_conventional_pages = 0;
VOID* kmem_largest_block = NULL;
UINTN kmem_largest_block_size = 0;
UINTN kmem_largest_block_page_count = 0;

EFI_PHYSICAL_ADDRESS* nebulae_system_table = NULL;

// The ultimate goal here, at the lowest level, is to have
// a per-NUMA node stack of available 2MB & 4k pages.
// Since we do not yet have the functionality to parse
// the ACPI tables that contain the information necessary
// to properly build those stacks of pages, we'll have
// to settle for 2 stacks right now, a 2MB page stack and
// a 4KB page stack
BOOLEAN physical_mem_stacks_allocated = FALSE;

kstack kmem_2MB_pages;
kstack kmem_4KB_pages;

// Get the count of a stack
UINT64 GetMemStackCount(UINT32 which_size) {
    switch (which_size) {
    case SIZE_4KB:
        return kGetStackCount(&kmem_4KB_pages);
    case SIZE_2MB:
        return kGetStackCount(&kmem_2MB_pages);
    default:
        return 0;
    }
}

// This function allocates 2MB of space for 2MB pages and 2MB of 
// space for 4KB pages in 2 separate kernel "stack" data structures
nebStatus InitMem() {
    
    UINT64 ret = NEB_OK;

    EFI_PHYSICAL_ADDRESS *base_2MB = AllocatePages(512);
    EFI_PHYSICAL_ADDRESS *base_4KB = AllocatePages(512);

    if (ISNULL(base_2MB) || ISNULL(base_4KB)) {
        return NEBERROR_STACK_MEM_ALLOC_ERR;
    }

    ret = kInitStackStructure(&kmem_2MB_pages, base_2MB, SIZE_2MB, KSTACK_DIRECTION_GROW_DOWN);

    if (NEB_ERROR(ret)) {
        return ret;
    }

    ret = kInitStackStructure(&kmem_4KB_pages, base_4KB, SIZE_2MB, KSTACK_DIRECTION_GROW_DOWN);

    if (NEB_ERROR(ret)) {
        return ret;
    }

    physical_mem_stacks_allocated = TRUE;
    return ret;
}

//
// Reads the memory map, performing the following tasks:
// 
//  1) Read UEFI memmap to obtain size of memory map
//  2) Allocate a buffer large enough to hold the memory map
//  3) Calculate the number of free conventional memory pages, along
//      with the size and location of the largest block of free
//      conventional memory
//  4) Query the UEFI memmap again
UINTN ReadUefiMemoryMap() {
    
    // Function exit status
    EFI_STATUS exit_status = EFI_SUCCESS;
    
    // If we've already left boot services, this call is worthless
    // Ditto if we haven't allocated any page stacks yet
    if (k0_main_called || !physical_mem_stacks_allocated) {
        Print(L"Bailed on ReadUefiMemoryMap, k0_main_called == %u, physical_mem_stacks_allocated == %u\n", 
            k0_main_called, 
            physical_mem_stacks_allocated);

        return 0;
    }

    // Vars required for BootServices->GetMemoryMap call
    EFI_STATUS memmap_status = EFI_SUCCESS;
    memmap.size = 0;
    memmap.key = 0;
    memmap.descr_size = 0;
    memmap.descr_version = 0;
    
    // Call GetMemoryMap first with a NULL pointer to get the (rough) size of the memory map
    memmap_status = gBS->GetMemoryMap(&memmap.size, 
        NULL, 
        &memmap.key, 
        &memmap.descr_size, 
        &memmap.descr_version);

    // Check for an error, but make sure that error is simply that the buffer
    // wasn't too small, because it was.
    if (EFI_ERROR(memmap_status) && memmap_status != EFI_BUFFER_TOO_SMALL) {
        kernel_panic(L"Unable to query UEFI for memory map size: %r\n", memmap_status);
    }

    if (k0_VERBOSE_DEBUG) {
        Print(L"Map Size: %lu, Map Key: %lu, Desc. Size: %lu, Desc. Version: %u\n",
            memmap.size, memmap.key, memmap.descr_size, memmap.descr_version);
    }

    // Allocate enough memory to hold a temporary copy of the memory map
    memmap.page_count = (memmap.size / EFI_PAGE_SIZE) + 1;
    memmap.memory_map = (EFI_MEMORY_DESCRIPTOR*)AllocatePages(memmap.page_count);
    if (memmap.memory_map == NULL) {
        kernel_panic(L"Unable to allocate pages for memory map\n");
    }

    if (k0_VERBOSE_DEBUG) {
        Print(L"Allocated %u page(s)\n", memmap.page_count);
    }

    // Call GetMemoryMap again to get the actual memory map
    memmap_status = gBS->GetMemoryMap(&memmap.size, 
        memmap.memory_map, 
        &memmap.key, 
        &memmap.descr_size, 
        &memmap.descr_version);

    if (EFI_ERROR(memmap_status)) {
        kernel_panic(L"Unable to query UEFI for memory map: %r\n", memmap_status);
    }

    // Parse the UEFI memory map in order to count the available
    // conventional memory
    EFI_MEMORY_DESCRIPTOR *memmap_entry = NULL;
    UINT64 memmap_entries = memmap.size / memmap.descr_size;
    VOID *memmap_iter = memmap.memory_map;

    UINTN i;
    for (i = 0; i < memmap_entries; i++) {
        memmap_entry = (EFI_MEMORY_DESCRIPTOR *)memmap_iter;
        if (memmap_entry->Type == EFI_MEMORY_CONVENTIONAL) {
            kmem_conventional_pages += memmap_entry->NumberOfPages;

            // Begin pushing pages onto the free page stack
            UINT64 current_address = (UINT64)memmap_entry->PhysicalStart;
            UINT64 mem_block_end = current_address + ((UINT64)memmap_entry->NumberOfPages * (UINT64)EFI_PAGE_SIZE);

            while (current_address < mem_block_end) {
                UINT64 *stack_result = NULL;

                if ((current_address % SIZE_2MB) == 0 && (current_address + SIZE_2MB) <= mem_block_end) {
                    stack_result = kStackPush(&kmem_2MB_pages, current_address);
                    current_address += SIZE_2MB;

                    if (stack_result == NULL) {
                        kernel_panic(L"Problem pushing 2MB page @ 0x%lx to 2MB page stack @ 0x%x\n", 
                            current_address, 
                            kmem_2MB_pages);
                    }
                }
                else if ((current_address % SIZE_4KB) == 0 && (current_address + SIZE_4KB) <= mem_block_end) {
                    stack_result = kStackPush(&kmem_4KB_pages, current_address);
                    current_address += SIZE_4KB;

                    if (stack_result == NULL) {
                        kernel_panic(L"Problem pushing 4KB page @ 0x%lx to 4KB page stack @ 0x%x\n", 
                            current_address, 
                            kmem_4KB_pages);
                    }
                }
                else {
                    Print(L"Memory address did not meet either criteria: 0x%lx\n", current_address);
                    kernel_panic(L"mem_block_end == 0x%lx\n", mem_block_end);
                }
            }

            if (memmap_entry->NumberOfPages > kmem_largest_block_page_count) {
                kmem_largest_block_page_count = memmap_entry->NumberOfPages;
                kmem_largest_block_size = kmem_largest_block_page_count * EFI_PAGE_SIZE;
                kmem_largest_block = (VOID*)memmap_entry->PhysicalStart;
            }
        }

        memmap_iter = (CHAR8*)memmap_iter + memmap.descr_size;
    }

    if (k0_VERBOSE_DEBUG) {
        Print(L"Pages of Conventional Memory: %lu\n", kmem_conventional_pages);
        Print(L"Largest Block: %lu\n", kmem_largest_block);
        Print(L"Largest Block Size: %lu\n", kmem_largest_block_size);
        Print(L"Largest Block Page Count: %lu\n", kmem_largest_block_page_count);
        Print(L"Memory subsystem initialized\n");
    }

    return memmap.key;
}

// Allocate 8 kb of system memory at a random
// 4KB aligned address in conventional memory
VOID AllocateSystemStruct() {

#ifdef __NEBULAE_ARCH_X64
    x64AllocateSystemStruct();

    // We need to remove the pages we just "allocated" for the system
    // struct
    nebStatus remove_sys_struct_pages = RemovePageContainingAddr(nebulae_system_table);

    if (NEB_ERROR(remove_sys_struct_pages)) {
        // well, this just shouldn't happen
        kernel_panic(L"Unable to remove system struct pages from physical memory stacks: %ld\n", 
            remove_sys_struct_pages);
    }
    else {
        if (k0_VERBOSE_DEBUG) {
            Print(L"Removed system struct page from physical memory stacks\n");
        }
    }

    // If we removed a 4KB page, then we need to remove another, 
    // since we allocated a contiguous 8KB for the system table
    if (remove_sys_struct_pages == NEBSTATUS_REMOVED_4KB_PAGE) {
        remove_sys_struct_pages = RemovePageContainingAddr((UINT64)(nebulae_system_table + SIZE_4KB));

        if (NEB_ERROR(remove_sys_struct_pages)) {
            kernel_panic(L"Unable to remove second system struct page from physical memory stacks: %ld\n", 
                remove_sys_struct_pages);
        } 
        else {
            if (k0_VERBOSE_DEBUG) {
                Print(L"Removed second system struct page from physical memory stacks\n");
            }
        }
    }
#endif
}

// Removes a page from the system physical memory stacks
// containing the specified address
nebStatus RemovePageContainingAddr(UINT64 addr) {
    
    nebStatus found_in_2MB = kStackSwapValue(&kmem_2MB_pages, (UINT64)addr, SIZE_2MB);
    if (NEB_ERROR(found_in_2MB)) {

        nebStatus found_in_4KB = kStackSwapValue(&kmem_4KB_pages, (UINT64)addr, SIZE_4KB);

        if (NEB_ERROR(found_in_4KB)) {
            // We didn't find shit
            return NEBERROR_STACK_ELEMENT_NOT_FOUND;
        }
        else {
            // We can just discard the value, cause it ain't coming back!
            kStackPop(&kmem_4KB_pages);
            return NEBSTATUS_REMOVED_4KB_PAGE;
        }
    }
    else {
        // Ditto
        kStackPop(&kmem_2MB_pages);
        return NEBSTATUS_REMOVED_2MB_PAGE;
    }
}

// Returns the raw paging structure for a given
// virtual address (only in kernel address space)
UINT64* GetPageInfo(EFI_VIRTUAL_ADDRESS addr) {

#ifdef __NEBULAE_ARCH_X64
    x64GetPageInfo(addr);
#endif

}

// Pops a free physical page base address of the
// requested size off the respective stack
UINT64 GetPage(UINTN page_size) {
    
    if (page_size != SIZE_4KB && page_size != SIZE_2MB) {
        return 0;
    }

    if (page_size == SIZE_4KB) {
        return kStackPop(&kmem_4KB_pages);
    }
    else if (page_size == SIZE_2MB) {
        return kStackPop(&kmem_2MB_pages);
    }

    return 0;
}
