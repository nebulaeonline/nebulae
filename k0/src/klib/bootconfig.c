// Copyright (c) 2003-2018 RCH3 (nebulae@nebulae.online)
// Copyright (c) 2019 Nebulae Foundation, LLC. All rights reserved.
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

#include <Library/ShellLib.h>
#include <Library/UefiLib.h>
#include <Library/MemoryAllocationLib.h>

#include "../include/k0.h"
#include "../include/klib/kstring.h"

extern UINTN nebulae_system_table_reserved_bytes;

BOOLEAN ProcessBootConfig() {
    // Process our system configuration file (k0.json.config)

    // Open the file
    SHELL_FILE_HANDLE configFileHandle;
    EFI_STATUS config_file_result = ShellOpenFileByName(L"k0.config.json",
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

    if (configFileInfo->FileSize > SIZE_1MB) {
        ShellCloseFile(configFileHandle);
        kernel_panic(L"k0.config.json may not exceed 1MB in size! Actual size of k0.config.json == %lu\n",
            configFileInfo->FileSize);
    }
    else if (k0_PRECONFIG_DEBUG) {
        Print(L"k0.config.json FileSize == %lu\n", configFileInfo->FileSize);
    }

    // Allocate enough memory to hold the config file, max 1MB
    CHAR8* config_file_buffer = AllocatePages(config_file_pages);

    if (ISNULL(config_file_buffer)) {
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

    if (k0_PRECONFIG_DEBUG) {
        Print(L"Config file buffer location: 0x%x\n", config_file_buffer);
    }

    // 1st time with NULL to get the token count
    INT32 token_count = jsmn_parse(&jsonParser,
        config_file_buffer,
        config_file_read_size,
        NULL,
        0);

    if (k0_PRECONFIG_DEBUG) {
        Print(L"Located %d json tokens in k0.config.json\n", token_count);
    }

    // Allocate a page for json parsing tokens
    UINTN json_pages_to_allocate = ((sizeof(jsmntok_t) * token_count) / EFI_PAGE_SIZE) + 1;
    jsmntok_t *json_tokens = (jsmntok_t*)AllocatePages(json_pages_to_allocate);

    if (ISNULL(json_tokens)) {
        kernel_panic(L"Unable to allocate %lu page(s) to tokenize k0.config.json: %r\n",
            json_pages_to_allocate,
            config_file_read_result);
    }
    else if (k0_PRECONFIG_DEBUG) {
        Print(L"Allocated %lu page(s) for k0.config.json token data\n", json_pages_to_allocate);
    }

    // Debug
    if (k0_PRECONFIG_DEBUG) {
        Print(L"Calling jsmn_parse()\n");
    }

    if (k0_PRECONFIG_DEBUG) {
        Print(L"Config file buffer location: 0x%x\n", config_file_buffer);
    }

    // 2nd time with to actually tokenize
    // Re-init parser
    jsmn_init(&jsonParser);
    INT32 actual_token_count = jsmn_parse(&jsonParser,
        config_file_buffer,
        config_file_read_size,
        json_tokens,
        token_count);

    if (k0_PRECONFIG_DEBUG) {
        Print(L"k0.config.json token count (< 0 indicates error) == %d\n", actual_token_count);
    }

    // token_count < 0 indicates an error
    if (actual_token_count < 0) {
        Print(L"Failed to parse k0.config.json; ignoring\n");
        return FALSE;
    }
    else if (actual_token_count == 0) {
        Print(L"Top level element is not an object in k0.config.json; ignoring\n");
        return FALSE;
    }
    else if (k0_PRECONFIG_DEBUG) {
        Print(L"Located %lu json token(s) in k0.config.json\n", actual_token_count);
    }

    // Now check for important tokens
    // TODO separate the config file code
    UINTN current_json_element;

    for (current_json_element = 0; current_json_element < actual_token_count; ++current_json_element) {
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
        }
        else if (jsoneq(config_file_buffer, &json_tokens[current_json_element], "debug-boot-pause") == 0) {
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
        else if (jsoneq(config_file_buffer, &json_tokens[current_json_element], "pagetable-debug") == 0) {
            if (k0_PRECONFIG_DEBUG) {
                Print(L"pagetable-debug token located in k0.config.json\n");
            }
            if (jsoneq(config_file_buffer, &json_tokens[current_json_element + 1], "true") == 0) {
                k0_PAGETABLE_DEBUG = TRUE;
                Print(L"Pagetable debug mode enabled\n");
            }
            else if (jsoneq(config_file_buffer, &json_tokens[current_json_element + 1], "false") == 0) {
                k0_PAGETABLE_DEBUG = FALSE;
                Print(L"Pagetable debug mode disabled\n");
            }
        }
        else if (jsoneq(config_file_buffer, &json_tokens[current_json_element], "system-reserved-memory") == 0) {
            if (k0_PRECONFIG_DEBUG) {
                Print(L"system-reserved-memory token located in k0.config.json\n");
            }
            if (jsoneq(config_file_buffer, &json_tokens[current_json_element + 1], "SIZE_8KB") == 0) {
                nebulae_system_table_reserved_bytes = SIZE_8KB;
                Print(L"system-reserved-memory set to 8KB\n");
            }
            else if (jsoneq(config_file_buffer, &json_tokens[current_json_element + 1], "SIZE_2MB") == 0) {
                nebulae_system_table_reserved_bytes = SIZE_2MB;
                Print(L"system-reserved-memory set to 2MB\n");
            }
            else if (jsoneq(config_file_buffer, &json_tokens[current_json_element + 1], "SIZE_4MB") == 0) {
                nebulae_system_table_reserved_bytes = SIZE_4MB;
                Print(L"system-reserved-memory set to 4MB\n");
            }
            else if (jsoneq(config_file_buffer, &json_tokens[current_json_element + 1], "SIZE_8MB") == 0) {
                nebulae_system_table_reserved_bytes = SIZE_8MB;
                Print(L"system-reserved-memory set to 8MB\n");
            }
            else if (jsoneq(config_file_buffer, &json_tokens[current_json_element + 1], "SIZE_16MB") == 0) {
                nebulae_system_table_reserved_bytes = SIZE_16MB;
                Print(L"system-reserved-memory set to 16MB\n");
            }
        }
    }

    // Free our json token pages
    FreePages(json_tokens, json_pages_to_allocate);
    if (k0_PRECONFIG_DEBUG) {
        Print(L"Freed %lu page(s) for k0.config.json token data\n", 1);
    }

    // Free our config file buffer pages
    FreePages(config_file_buffer, config_file_pages);
    if (k0_PRECONFIG_DEBUG) {
        Print(L"Freed %lu page(s) for k0.config.json file data\n", 1);
    }

    return TRUE;
}
