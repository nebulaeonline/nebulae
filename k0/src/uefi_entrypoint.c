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

// UEFI Debug Library (ASSERT) & Boot Services
#include <Library/UefiLib.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>

// Memory allocation & shell lib file handling capabilities
#include <Library/MemoryAllocationLib.h>
#include <Library/ShellLib.h>

// Kernel Include(s)
#include "include/k0.h"
#include "include/jsmn.h"
#include "include/kstring.h"

// We run on any UEFI Specification
extern CONST UINT32 _gUefiDriverRevision;
CONST UINT32 _gUefiDriverRevision = 0;

// Module name
CHAR8 *gEfiCallerBaseName = "k0";

// Debugging flags
BOOLEAN k0_VERBOSE_DEBUG = FALSE;       // Set by configuration file
BOOLEAN k0_PRECONFIG_DEBUG = TRUE;      // Only set when debugging pre-config file load

// Helper function for json string matching without libc
static int jsoneq(const char *json, jsmntok_t *tok, const char *s) {
    if (tok->type == JSMN_STRING && (int)kStrlen(s) == tok->end - tok->start &&
        kStrnCmpA(json + tok->start, s, tok->end - tok->start) == 0) {
        return 0;
    }
    return -1;
}

// Placeholder
EFI_STATUS EFIAPI UefiUnload(IN EFI_HANDLE image_handle) {
    
    // This code should be compiled out and never called 
    ASSERT(FALSE);
    ASSERT(image_handle != NULL);
    return EFI_SUCCESS;
}

// Declare Kernel Entrypoint
NORETURN void kernel_main(void);

// Declare Global Pointers to UEFI tables
EFI_HANDLE nebulae_uefi_image_handle;
EFI_SYSTEM_TABLE* nebulae_uefi_system_table;

// UEFI Entrypoint
EFI_STATUS EFIAPI UefiMain(IN EFI_HANDLE image_handle, IN EFI_SYSTEM_TABLE* system_table) {
    
    ASSERT(image_handle != NULL);
    ASSERT(system_table != NULL);

    // Disable UEFI watchdog timer
    gBS->SetWatchdogTimer(0, 0, 0, NULL);

    // Announce 
    Print(L"Booting nebulae!\n");
    
    // Set our global UEFI handle & pointer
    nebulae_uefi_image_handle = image_handle;
    nebulae_uefi_system_table = system_table;

    // Initialize the shell for the duration
    // of the boot process
    EFI_STATUS shell_status = ShellInitialize();
    if (EFI_ERROR(shell_status))
    {
        kernel_panic(L"Failed to initialize the UEFI shell: %r\n", shell_status);
    }

    // Process our system configuration file (k0.json.config)
    
    // Open the file
    SHELL_FILE_HANDLE configFileHandle;
    EFI_STATUS config_file_result = ShellOpenFileByName(L"fs1:\\k0.config.json",
        &configFileHandle,
        EFI_FILE_MODE_READ,
        0);
    
    if (EFI_ERROR(config_file_result)) {
        configFileHandle = NULL;
        kernel_panic(L"Failed to open k0 configuration file: %r\n", config_file_result);
    }
    else if (k0_PRECONFIG_DEBUG) {
        Print(L"Opened k0.config.json\n");
    }

    // Determine the file size
    EFI_FILE_INFO *configFileInfo = ShellGetFileInfo(configFileHandle);
    UINTN config_file_pages = (configFileInfo->FileSize / EFI_PAGE_SIZE) + 1;
    
    if (configFileInfo->FileSize > SIZE_4KB) {
        ShellCloseFile(configFileHandle);
        kernel_panic(L"k0.config.json may not exceed 4KB in size! Actual size of k0.config.json == %lu\n", configFileInfo->FileSize);
    }
    else if (k0_PRECONFIG_DEBUG) {
        Print(L"k0.config.json FileSize == %lu\n", configFileInfo->FileSize);
    }

    // Allocate enough memory to hold the config file, max 1MB
    CHAR8* config_file_buffer = AllocatePages(config_file_pages);

    if (config_file_buffer == NULL) {
        kernel_panic(L"Failed to allocate pages to process k0 configuration file. Allocation operation returned NULL.\n");
    }
    else if (k0_PRECONFIG_DEBUG) {
        Print(L"Allocated %lu pages for k0.config.json file data\n", config_file_pages);
    }

    UINTN config_file_read_size = configFileInfo->FileSize;
    EFI_STATUS config_file_read_result = ShellReadFile(configFileHandle, &config_file_read_size, config_file_buffer);
    
    if (EFI_ERROR(config_file_read_result)) {
        kernel_panic(L"Failed to read k0.config.json: %r\n", config_file_read_result);
    }
    else if (k0_PRECONFIG_DEBUG) {
        Print(L"Read k0.config.json into memory\n");
    }

    // Close the file
    EFI_STATUS efi_close_result = ShellCloseFile(&configFileHandle);
    if (EFI_ERROR(efi_close_result)) {
        kernel_panic(L"Unable to close k0.config.json\n");
    }
    else if (k0_PRECONFIG_DEBUG) {
        Print(L"Closed k0.config.json\n");
    }
  
    // Call the actual json token parsing library
    jsmn_parser jsonParser;
    jsmn_init(&jsonParser);

    // Allocate a page for json parsing tokens
    jsmntok_t *json_tokens = (jsmntok_t*)AllocatePages(1);

    if (json_tokens == NULL) {
        kernel_panic(L"Unable to allocate pages to tokenize k0.config.json: %r\n", config_file_read_result);
    }
    else if (k0_PRECONFIG_DEBUG) {
        Print(L"Allocated %lu page(s) for k0.config.json token data\n", 1);
    }

    // Debug
    if (k0_PRECONFIG_DEBUG) {
        Print(L"Calling jsmn_parse()\n");
    }

    // 1st time with NULL to get the token count
    UINT32 token_count = jsmn_parse(&jsonParser,
        config_file_buffer,
        config_file_read_size,
        json_tokens,
        64);

    if (k0_PRECONFIG_DEBUG) {
        Print(L"k0.config.json token count (< 0 indicates error) == %lu\n", token_count);
    }

    // token_count < 0 indicates an error
    if (token_count < 0) {
        Print(L"Failed to parse k0.config.json; ignoring\n");
        goto kernel_entry;
    }
    else if (token_count == 0) {
        Print(L"Top level element is not an object in k0.config.json; ignoring\n");
        goto kernel_entry;
    }
    else if (k0_PRECONFIG_DEBUG) {
        Print(L"Located %lu json token(s) in k0.config.json\n", token_count);
    }

    // Now check for important tokens
    // TODO separate the config file code
    UINTN current_json_element;

    for (current_json_element = 0; current_json_element < token_count; ++current_json_element) {
        if (jsoneq(config_file_buffer, &json_tokens[current_json_element], "debug") == 0) {
            if (k0_PRECONFIG_DEBUG) {
                Print(L"Debug token located in k0.config.json\n");
            }
            if (jsoneq(config_file_buffer, &json_tokens[current_json_element + 1], "true") == 0) {
                k0_VERBOSE_DEBUG = TRUE;
                Print(L"Verbose kernel debugging enabled\n");
            }
            else if (jsoneq(config_file_buffer, &json_tokens[current_json_element + 1], "false") == 0) {
                k0_VERBOSE_DEBUG = FALSE;
                Print(L"Verbose kernel debugging disabled\n");
            }
        } else if (jsoneq(config_file_buffer, &json_tokens[current_json_element], "debug-boot-pause") == 0) {
            if (k0_PRECONFIG_DEBUG) {
                Print(L"Debug-boot-pause token located in k0.config.json\n");
            }
            if (jsoneq(config_file_buffer, &json_tokens[current_json_element + 1], "true") == 0) {
                ShellSetPageBreakMode(TRUE);
                Print(L"Shell page-break mode enabled for boot\n");
            }
            else if (jsoneq(config_file_buffer, &json_tokens[current_json_element + 1], "false") == 0) {
                ShellSetPageBreakMode(FALSE);
                Print(L"Shell page-break mode disabled for boot\n");
            }
        }
    }

    // Free our json token pages
    FreePages(json_tokens, 1);
    if (k0_PRECONFIG_DEBUG) {
        Print(L"Freed %lu page(s) for k0.config.json token data\n", 1);
    }

    // Free our config file buffer pages
    FreePages(config_file_buffer, config_file_pages);
    if (k0_PRECONFIG_DEBUG) {
        Print(L"Freed %lu page(s) for k0.config.json file data\n", 1);
    }

    // Call our kernel_main() function
kernel_entry:
    kernel_main();

exit:
    return EFI_SUCCESS;
}
