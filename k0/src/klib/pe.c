// Copyright (c) 2005-2019 Nebulae Foundation. All rights reserved.
//
//   https://docs.microsoft.com/en-us/windows/desktop/debug/pe-format
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

#include "../include/klib/pe.h"

VOID UefiLoadPEFile(CHAR16 *filename, uefi_file *fp) {
    // Open the file
    EFI_STATUS exec_file_result = ShellOpenFileByName(filename,
        &fp->file_handle,
        EFI_FILE_MODE_READ,
        0);

    if (EFI_ERROR(exec_file_result)) {
        fp->file_handle = NULL;
        kernel_panic(L"Failed to open executable file %s: %r\n", filename, exec_file_result);
    }
    else if (k0_PRECONFIG_DEBUG) {
        Print(L"Opened %s\n", filename);
    }

    // Determine the file size
    EFI_FILE_INFO *exec_file_info = ShellGetFileInfo(fp->file_handle);
    UINTN exec_file_pages = (exec_file_info->FileSize / EFI_PAGE_SIZE) + 1;

    // Allocate enough memory to hold the exec file
    fp->buffer = AllocatePages(exec_file_pages);

    if (ISNULL(fp->buffer)) {
        kernel_panic(L"Failed to allocate pages to load exec file %s. Allocation operation returned NULL.\n", filename);
    }
    else if (k0_VERBOSE_DEBUG) {
        Print(L"Allocated %lu pages for %s file data\n", exec_file_pages, filename);
    }

    fp->size = exec_file_info->FileSize;
    EFI_STATUS exec_file_read_result = ShellReadFile(fp->file_handle, &fp->size, fp->buffer);

    if (EFI_ERROR(exec_file_read_result)) {
        kernel_panic(L"Failed to read %s: %r\n", filename, exec_file_read_result);
    }
    else if (k0_VERBOSE_DEBUG) {
        Print(L"Read %s into memory\n", filename);
    }

    // Close the file -- later
    /*
    EFI_STATUS efi_close_result = ShellCloseFile(&exec_file_handle);
    if (EFI_ERROR(efi_close_result)) {
        kernel_panic(L"Unable to close %s\n", filename);
    }
    else if (k0_VERBOSE_DEBUG) {
        Print(L"Closed %s\n", filename);
    }
    */
}
