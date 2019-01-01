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

//
// Basic UEFI Libraries
//
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/BaseMemoryLib.h>
//
// Boot and Runtime Services
//
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>

//
// Shell Library
//
#include <Library/ShellLib.h>

// 
// Kernel Headers
//
#include "include/k.h"
#include "include/kmem.h"

//
// We run on any UEFI Specification
// **defining in zig
extern CONST UINT32 _gUefiDriverRevision = 0;

//
// Module name
//
CHAR8 *gEfiCallerBaseName = "k";

//
// Debugging Toggle
//
CONST BOOLEAN kDBG = KDBG;


EFI_STATUS EFIAPI UefiUnload(IN EFI_HANDLE image_handle) {
    
    // 
    // This code should be compiled out and never called 
    // 
    ASSERT(FALSE);
}

EFI_STATUS EFIAPI UefiMain(IN EFI_HANDLE image_handle, IN EFI_SYSTEM_TABLE* system_table) {
    
    EFI_STATUS exit_status = EFI_SUCCESS;
    EFI_STATUS shell_status = EFI_SUCCESS;

    // Disable UEFI watchdog timer
    system_table->BootServices->SetWatchdogTimer(0, 0, 0, NULL);

    // Announce 
    Print(L"Welcome to nebulae!\n");

    // Init the shell for scroll breaking capability
    shell_status = ShellInitialize();
    if (EFI_ERROR(shell_status))
    {
        Print(L"Failed to initialize shell: %lx\n", shell_status);
        exit_status = shell_status;
        goto exit;
    }
    ShellSetPageBreakMode(TRUE);

    // Init memory subsystem
    InitMemSubsystem();


exit:
    return exit_status;
}
