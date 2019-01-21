// Copyright (c) 2003-2019 Nebulae Foundation. All rights reserved.
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
#include <Library/ShellLib.h>
#include <Library/UefiBootServicesTableLib.h>

// Kernel includes
#include "include/k0.h"

// nebulae-Uefi Headers
#include "include/arch/uefi/memory.h"
#include "include/arch/uefi/graphics.h"
#include "include/arch/uefi/panic.h"

#include "include/klib/kstring.h"
#include "include/klib/interrupt.h"

// Main kernel page tables
preboot_mem_block k0_boot_scratch_area;
x64_virtual_address_space k0_addr_space;

// Main system table
nebulae_system_table *system_table = NULL;

// Have we been called?
BOOLEAN k0_main_called = FALSE;

// Kernel Entrypoint
// Graphics are initialized, we're flying solo!
NORETURN VOID k0_main(VOID) {
    
    // We're here!
    k0_main_called = TRUE;

    // Do something
    Print(L"entered main nebulae kernel...\n");

    // Initialize interrupts
    InitInterrupts();

    /*
    if (k0_VERBOSE_DEBUG) {
        Print(L"system_table->magic === 0x%lx\n", system_table->magic);
        Print(L"system_table->version_major == 0x%x\n", system_table->version_major);
        Print(L"system_table->version_minor == 0x%x\n", system_table->version_minor);
        Print(L"system_table->version_build == 0x%lx\n", system_table->version_build);
        Print(L"system_table->data_offset == 0x%x\n", system_table->data_offset);
        Print(L"system_table->next_free_byte == 0x%lx\n", system_table->next_free_byte);
        Print(L"system_table->free_space == 0x%lx\n", system_table->free_space);
    }
    */

    // Draw a blue triangle to the screen
    drawTriangle(gfx_info.gop->Mode->FrameBufferBase, 100, 100, 75, 0x000000ff);

    // Woo-hoo!
    while (TRUE) {}
}
