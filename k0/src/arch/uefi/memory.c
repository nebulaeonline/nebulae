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
#include "../../include/klib/kstring.h"

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
UINTN kmem_total_page_count = 0;

extern nebulae_system_table *system_table;
UINTN nebulae_system_table_reserved_bytes = NEBULAE_SYSTEM_TABLE_RESERVED_BYTES;

// The ultimate goal here, at the lowest level, is to have
// a per-NUMA node stack of available 2MB & 4k pages.
// Since we do not yet have the functionality to parse
// the ACPI tables that contain the information necessary
// to properly build those stacks of pages, we'll have
// to settle for 2 stacks right now, a 2MB page stack and
// a 4KB page stack
BOOLEAN physical_mem_stacks_allocated = FALSE;

kstack kmem_free_pages_2MB;
kstack kmem_free_pages_4KB;

// Allocates a page of the requested page size
// *** THIS PAGE IS NOT ZEROED HERE -- BEWARE!! ***
EFI_PHYSICAL_ADDRESS* AllocPhysicalPage(UINTN page_size) {
    EFI_PHYSICAL_ADDRESS *new_page_base = NULL;

    switch (page_size) {
    case SIZE_4KB:
        new_page_base = (EFI_PHYSICAL_ADDRESS *)kStackPop(&kmem_free_pages_4KB);
        if (ISNULL(new_page_base)) {
            return NULL;
        }
        break;
    case SIZE_2MB:
        new_page_base = (EFI_PHYSICAL_ADDRESS *)kStackPop(&kmem_free_pages_2MB);
        if (ISNULL(new_page_base)) {
            return NULL;
        }
        break;
    default:
        return NULL;
    }

    return (EFI_PHYSICAL_ADDRESS*)new_page_base;
}

// Allocates a physical page of memory that contains the
// specified address; If NULL is returned, page_size must 
// be ignored on return
EFI_PHYSICAL_ADDRESS* AllocPageContainingPhysicalAddr(EFI_PHYSICAL_ADDRESS *addr, OUT UINTN *page_size) {
    
    // Address and page_size pointer cannot be NULL
    if (ISNULL(addr) || ISNULL(page_size)) {
        return NULL;
    }
    
    // Look in 2MB pages first
    nstatus swap_result = kStackSwapValue(&kmem_free_pages_2MB, (UINT64)addr, SIZE_2MB);
    if (NEB_ERROR(swap_result)) { // not found, look in 4KB pages
        swap_result = kStackSwapValue(&kmem_free_pages_4KB, (UINT64)addr, SIZE_4KB);
        if (NEB_ERROR(swap_result)) { // really not found
            return NULL;
        }
        else {
            *page_size = SIZE_4KB;
            return (EFI_PHYSICAL_ADDRESS *)kStackPop(&kmem_free_pages_4KB);
        }
    }
    else {
        *page_size = SIZE_2MB;
        return (EFI_PHYSICAL_ADDRESS *)kStackPop(&kmem_free_pages_2MB);
    }
}

// Frees a physical page of memory allocated with AllocPhysicalPage
// *** THIS PAGE IS NOT ZEROED HERE -- BEWARE!! ***
// #TODO free physical pages (and keep track of them)
nstatus FreePhysicalPage(EFI_PHYSICAL_ADDRESS *base_addr, UINTN page_size) {
    /*
    if (page_size != SIZE_4KB && page_size != SIZE_2MB) {
        Print(L"Invalid page size\n");
        return NEBERROR_INVALID_PAGE_SIZE;
    }

    nstatus swap_result = NEBSTATUS_UNDETERMINED;
    UINT64 allocated_result = 0;
    */

    return NEB_OK;
}

// Get the count of a free memory stack
UINT64 GetFreeMemStackCount(UINT32 which_size) {
    switch (which_size) {
    case SIZE_4KB:
        return kGetStackCount(&kmem_free_pages_4KB);
    case SIZE_2MB:
        return kGetStackCount(&kmem_free_pages_2MB);
    default:
        return 0;
    }
}

// This function allocates 2MB of space for 2MB pages and 2MB of 
// space for 4KB pages in 2 separate kernel "stack" data structures
nstatus InitMem() {
    
    UINT64 ret = NEB_OK;

    EFI_PHYSICAL_ADDRESS *base_free_2MB = AllocatePages(512);
    EFI_PHYSICAL_ADDRESS *base_free_4KB = AllocatePages(512);
    EFI_PHYSICAL_ADDRESS *base_alloc_2MB = AllocatePages(512);
    EFI_PHYSICAL_ADDRESS *base_alloc_4KB = AllocatePages(512);

    if (ISNULL(base_free_2MB) || ISNULL(base_free_4KB) ||
        ISNULL(base_alloc_2MB) || ISNULL(base_alloc_4KB)) {
        return NEBERROR_STACK_MEM_ALLOC_ERR;
    }

    ret = kInitStackStructure(&kmem_free_pages_2MB, base_free_2MB, SIZE_2MB, KSTACK_DIRECTION_GROW_DOWN);

    if (NEB_ERROR(ret)) {
        return ret;
    }

    ret = kInitStackStructure(&kmem_free_pages_4KB, base_free_4KB, SIZE_2MB, KSTACK_DIRECTION_GROW_DOWN);

    if (NEB_ERROR(ret)) {
        return ret;
    }

    physical_mem_stacks_allocated = TRUE;
    return ret;
}

// Sets up a preboot memory block
preboot_mem_block* InitPrebootMemBlock(preboot_mem_block *pbmb, CHAR16 id[5], VOID *base_addr, UINTN block_size) {
    if (ISNULL(pbmb) || ISNULL(base_addr) || block_size == 0) {
        return NULL;
    }

    pbmb->base_addr = base_addr;
    pbmb->current_addr = base_addr;
    pbmb->size = block_size;
    pbmb->free_space = block_size;
    pbmb->wasted_space = 0;
    CopyMem(&pbmb->id, &id, 10);
    pbmb->id[4] = L'\0';
    return pbmb;
}

// Basically a one-way malloc without free for preboot
VOID* kPrebootMalloc(preboot_mem_block *pbmb, UINTN allocation_size, UINT64 desired_alignment) {

    // No NULLs
    if (ISNULL(pbmb) || allocation_size == 0) {
        return NULL;
    }

    // Calculate our new address given the requested alignment
    UINT64 return_addr = ALIGN_UP((UINT64)pbmb->current_addr, desired_alignment);

    // Do we have the space to account for the requested alignment and the size?
    if ((return_addr + allocation_size) > ((UINT64)pbmb->current_addr + (UINT64)pbmb->free_space)) {
        return NULL;
    } 
    
    pbmb->wasted_space += (return_addr - (UINT64)pbmb->current_addr);

    pbmb->current_addr = (VOID *)(return_addr + allocation_size);
    pbmb->free_space = (((UINT64)pbmb->base_addr + pbmb->size) - (UINT64)pbmb->current_addr);
    
    return (VOID*)return_addr;
}

// A critical version of kPrebootMalloc()
VOID* kPrebootCriticalMalloc(preboot_mem_block *pbmb, UINTN allocation_size, UINT64 desired_alignment) {

    VOID* kpm_new = kPrebootMalloc(pbmb, allocation_size, desired_alignment);

    if (ISNULL(kpm_new)) {
        kernel_panic(L"Critical memory allocation failed in preboot environment: %s\n", pbmb->id);
    }

    if (ZeroMem(kpm_new, allocation_size) != kpm_new) {
        kernel_panic(L"Error clearing memory during a critical allocation in preboot environment: %s\n", pbmb->id);
    }

    return kpm_new;
}

// Dumps the memory map to the screen
VOID DumpUefiMemoryMap() {
    Print(L"Map Size: %lu, Map Key: %lu, Desc. Size: %lu, Desc. Version: %u\n",
        memmap.size, memmap.key, memmap.descr_size, memmap.descr_version);

    // Parse the UEFI memory map in order to count the available
    // conventional memory
    EFI_MEMORY_DESCRIPTOR *memmap_entry = NULL;
    UINT64 memmap_entries = memmap.size / memmap.descr_size;
    VOID *memmap_iter = memmap.memory_map;

    UINTN i;
    for (i = 0; i < memmap_entries; i++) {
        memmap_entry = (EFI_MEMORY_DESCRIPTOR *)memmap_iter;

        Print(L"memmap_entry type: 0x%lx pstart: 0x%lx pages: 0x%lx attr: 0x%lx\n",
            memmap_entry->Type,
            memmap_entry->PhysicalStart,
            memmap_entry->NumberOfPages,
            memmap_entry->Attribute);

        memmap_iter = (CHAR8*)memmap_iter + memmap.descr_size;
    }
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
    if (ISNULL(memmap.memory_map)) {
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
        kmem_total_page_count += memmap_entry->NumberOfPages;

        if (memmap_entry->Type == EFI_MEMORY_CONVENTIONAL) {
            kmem_conventional_pages += memmap_entry->NumberOfPages;

            // Begin pushing pages onto the free page stack
            UINT64 current_address = (UINT64)memmap_entry->PhysicalStart;
            UINT64 mem_block_end = current_address + ((UINT64)memmap_entry->NumberOfPages * (UINT64)EFI_PAGE_SIZE);

            while (current_address < mem_block_end) {
                UINT64 *stack_result = NULL;

                if ((current_address % SIZE_2MB) == 0 && (current_address + SIZE_2MB) <= mem_block_end) {
                    stack_result = kStackPush(&kmem_free_pages_2MB, current_address);
                    current_address += SIZE_2MB;

                    if (ISNULL(stack_result)) {
                        kernel_panic(L"Problem pushing 2MB page @ 0x%lx to 2MB page stack @ 0x%x\n", 
                            current_address, 
                            kmem_free_pages_2MB);
                    }
                }
                else if ((current_address % SIZE_4KB) == 0 && (current_address + SIZE_4KB) <= mem_block_end) {
                    stack_result = kStackPush(&kmem_free_pages_4KB, current_address);
                    current_address += SIZE_4KB;

                    if (ISNULL(stack_result)) {
                        kernel_panic(L"Problem pushing 4KB page @ 0x%lx to 4KB page stack @ 0x%x\n", 
                            current_address, 
                            kmem_free_pages_4KB);
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
        //Print(L"Memory subsystem initialized\n");
    }

    DumpUefiMemoryMap();

    return memmap.key;
}

// Allocate 8 kb of system memory at a random
// 4KB aligned address in conventional memory
VOID AllocateSystemStruct() {

#ifdef __NEBULAE_ARCH_X64
    x64AllocateSystemStruct();
#endif

    system_table->magic = NEBULAE_SIG;
    system_table->version_major = NEBULAE_VERSION_MAJOR;
    system_table->version_minor = NEBULAE_VERSION_MINOR;
    system_table->version_build = NEBULAE_VERSION_BUILD;
 }

// Removes a page from the free system physical memory stacks
// containing the specified address
nstatus RemoveFreePageContainingPhysicalAddr(UINT64 addr) {
    
    nstatus found_in_2MB = kStackSwapValue(&kmem_free_pages_2MB, (UINT64)addr, SIZE_2MB);
    if (NEB_ERROR(found_in_2MB)) {

        nstatus found_in_4KB = kStackSwapValue(&kmem_free_pages_4KB, (UINT64)addr, SIZE_4KB);

        if (NEB_ERROR(found_in_4KB)) {
            // We didn't find shit
            if (k0_VERBOSE_DEBUG) {
                Print(L"Unable to locate requested address @ 0x%lx\n", addr);
            }
            return NEBERROR_STACK_ELEMENT_NOT_FOUND;
        }
        else {
            // We can just discard the value, cause it ain't coming back!
            kStackPop(&kmem_free_pages_4KB);
            return NEBSTATUS_REMOVED_4KB_PAGE;
        }
    }
    else {
        // Ditto
        kStackPop(&kmem_free_pages_2MB);
        return NEBSTATUS_REMOVED_2MB_PAGE;
    }
}

// Is a page in one of the free page stacks?
// This function is slow, and is not meant to 
// be used except during preboot;  This WILL
// cause a race condition if used once 
// multiprocessing is enabled.
BOOLEAN IsPageFree_Preboot(UINT64 addr) {
    nstatus found_in_2MB = kStackFindValue(&kmem_free_pages_2MB, (UINT64)addr, SIZE_2MB);
    if (NEB_ERROR(found_in_2MB)) {

        nstatus found_in_4KB = kStackFindValue(&kmem_free_pages_4KB, (UINT64)addr, SIZE_4KB);

        if (NEB_ERROR(found_in_4KB)) {
            // We didn't find shit
            return FALSE;
        }
        else {
            return TRUE;
        }
    }
    else {
        return TRUE;
    }
}

// Returns the raw paging structure for a given
// virtual address (only in kernel address space)
UINT64* GetPageInfo(EFI_VIRTUAL_ADDRESS addr) {

#ifdef __NEBULAE_ARCH_X64
    return x64GetPageInfo(NULL, addr);
#endif

}

// Invalidates a page in the page cache (TLB)
VOID InvalidatePage(EFI_VIRTUAL_ADDRESS *base_addr) {
#ifdef __NEBULAE_ARCH_X64
    x64InvalidatePage(base_addr);
#endif
}