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

// UEFI Debug Library (ASSERT) & Boot Services
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>

// Memory allocation & shell lib file handling capabilities
#include <Library/MemoryAllocationLib.h>
#include <Library/ShellLib.h>

// Kernel Include(s)
#include "../../include/k0.h"
#include "../../include/deps/jsmn.h"
#include "../../include/klib/kstring.h"
#include "../../include/klib/bootconfig.h"

#include "../../include/arch/uefi/memory.h"
#include "../../include/arch/x64/acpi.h"

// We run on any UEFI Specification
extern CONST UINT32 _gUefiDriverRevision;
CONST UINT32 _gUefiDriverRevision = 0;

// Module name
CHAR8 *gEfiCallerBaseName = "k0";

// Boot Time Debugging flags
BOOLEAN k0_VERBOSE_DEBUG = FALSE;       // Set by configuration file
BOOLEAN k0_PRECONFIG_DEBUG = TRUE;      // Only set when debugging pre-config file load
BOOLEAN k0_PAGETABLE_DEBUG = FALSE;     // Only set when debugging pagetables

// Placeholder
EFI_STATUS EFIAPI UefiUnload(IN EFI_HANDLE image_handle) {
    
    // This code should be compiled out and never called 
    ASSERT(FALSE);
    ASSERT(image_handle != NULL);
    return EFI_SUCCESS;
}

// Declare Kernel Entrypoint
NORETURN VOID k0_main();

// UEFI Entrypoint
EFI_STATUS EFIAPI UefiMain(IN EFI_HANDLE uefi_image_handle, IN EFI_SYSTEM_TABLE* uefi_system_table) {
    
    ASSERT(uefi_image_handle != NULL);
    ASSERT(uefi_system_table != NULL);

    // Disable UEFI watchdog timer
    gBS->SetWatchdogTimer(0, 0, 0, NULL);

    // Announce 
    Print(L"Booting nebulae!\n");
    
    // Initialize the shell for the duration
    // of the boot process
    EFI_STATUS shell_status = ShellInitialize();
    if (EFI_ERROR(shell_status))
    {
        kernel_panic(L"Failed to initialize the UEFI shell: %r\n", shell_status);
    }

    // Read the boot configuration file
    ProcessBootConfig();

    // Initialize the Isaac64 CSPRNG
    // Isaac64 generates a 64-bit cryptographically
    // secure pseudo-random number in 19 cycles 
    // on x64
    InitIsaac64CSPRNG(TRUE);

    // Initialize the cpu architecture
#ifdef __NEBULAE_ARCH_X64    
    x64InitCpuInfoStructs();

    // Initialize SSE & beyond
    if (!x64ReadCpuinfoFlags(X64_HAS_SSE)) {
        kernel_panic(L"nebulae requires a system that supports SSE instructions\n");
    }

    if (!x64ReadCpuinfoFlags(X64_HAS_SSE2)) {
        kernel_panic(L"nebulae requires a system that supports SSE2 instructions\n");
    }

    if (!x64ReadCpuinfoFlags(X64_HAS_SSE3)) {
        kernel_panic(L"nebulae requires a system that supports SSE3 instructions\n");
    }

    if (!x64ReadCpuinfoFlags(X64_HAS_SSE41)) {
        kernel_panic(L"nebulae requires a system that supports SSE4.1 instructions\n");
    }

    if (!x64ReadCpuinfoFlags(X64_HAS_SSE42)) {
        kernel_panic(L"nebulae requires a system that supports SSE4.2 instructions\n");
    }

    if (!x64ReadCpuinfoFlags(X64_HAS_FXSAVE)) {
        kernel_panic(L"nebulae requires a system that supports FXSAVE/FXRSTOR instructions\n");
    }

    /*
    if (!x64ReadCpuinfoFlags(X64_HAS_POPCNT)) {
        kernel_panic(L"nebulae requires a system that supports the POPCNT instruction\n");
    }

    
    if (!x64ReadCpuinfoFlags(X64_HAS_OSXSAVE)) {
        kernel_panic(L"nebulae requires a system that supports OSXSAVE instructions atm\n");
    }
    */
#endif

    // Change the graphics mode
    Print(L"Changing graphics mode\n");

    // Disable scrolling
    ShellSetPageBreakMode(FALSE);

    // Initialize the graphics engine to 1024x768
    InitGraphics();

    // Initialize the memory subsystem
    nebStatus init_mem_status = InitMem();

    if (NEB_ERROR(init_mem_status)) {
        kernel_panic(L"There was a problem initializing the memory management subsystem: %ld\n", init_mem_status);
    }
    else if (k0_VERBOSE_DEBUG) {
        Print(L"Physical memory page stacks initialized\n");
    }

    // Read the UEFI memory map
    UINTN uefi_mem_key = ReadUefiMemoryMap();
    
    Print(L"Exiting UEFI Boot Services!\n");
    
    // Allocate our boot scratch memory area (pre-paging still)
#ifdef __NEBULAE_ARCH_X64
    x64AllocateBootScratchArea();    
#endif

    // Set aside our system data structure
    extern nebulae_system_table *system_table;
    AllocateSystemStruct();
    Print(L"System struct allocated at %lx\n", system_table);

    // Setup our system struct
    if (system_table->magic != NEBULAE_SIG) {
        kernel_panic(L"System table allocation signature mismatch\n");
    }

    system_table->uefi_image_handle = uefi_image_handle;
    system_table->uefi_system_table = uefi_system_table;

    // Locate the ACPI XSDT tables, and parse the MADT
    // table to determine the # of cpus and ioapic/lapic info
#ifdef __NEBULAE_ARCH_X64
    x64PreInitAcpi();
#endif

    // Exit Boot Services and start flying solo
    gBS->ExitBootServices(uefi_image_handle, uefi_mem_key);
    gST->RuntimeServices->SetVirtualAddressMap(memmap.size, memmap.descr_size, memmap.descr_version, memmap.memory_map);
    
    // Create our page tables
#ifdef __NEBULAE_ARCH_X64
    x64InitKernelStacks();
    x64InitGDT();
    x64InitIDT();
    x64InitPIC();
    x64InitLocalApic();
    x64InitIoApic();
    x64BuildInitialKernelPageTable();
#endif
    
    // Allocate and free pages -- BEGIN MEMORY TEST
    EFI_PHYSICAL_ADDRESS *page_2MB = AllocPhysicalPage(SIZE_2MB);
    if (ISNULL(page_2MB)) {
        kernel_panic(L"Unable to allocate 2MB page\n");
    }
    EFI_PHYSICAL_ADDRESS *page_4KB = AllocPhysicalPage(SIZE_4KB);
    if (ISNULL(page_4KB)) {
        kernel_panic(L"Unable to allocate 4KB page\n");
    }

    nebStatus free_result = FreePhysicalPage(page_2MB, SIZE_2MB);

    if (NEB_ERROR(free_result)) {
        kernel_panic(L"Unable to free 2MB page; result == %ld\n", free_result);
    }

    free_result = FreePhysicalPage(page_4KB, SIZE_4KB);

    if (NEB_ERROR(free_result)) {
        kernel_panic(L"Unable to free 4KB page; result == %ld\n", free_result);
    }

    Print(L"Memory subsystem integrity testing complete\n");
    // -- END MEMORY TEST

    // Inform the user about our memory situation
    UINT64 count_free_pages_2MB = GetFreeMemStackCount(SIZE_2MB);
    UINT64 count_free_pages_4KB = GetFreeMemStackCount(SIZE_4KB);

    Print(L"Located %lu 2MB pages of available conventional physical memory\n", count_free_pages_2MB);
    Print(L"Located %lu 4KB pages of available conventional physical memory\n", count_free_pages_4KB);

    Print(L"Total available physical memory == %lu KB\n", 
        ((count_free_pages_2MB * SIZE_2MB) + (count_free_pages_4KB * SIZE_4KB)) / 1024);

    k0_main();

exit:
    return EFI_SUCCESS;
}
