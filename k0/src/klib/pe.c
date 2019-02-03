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

#include <Library/BaseMemoryLib.h>
#include <Library/DebugLib.h>


// This function will load the specified PE file using UEFI facilities,
// and panic on failure; Returns a pointer to the entrypoint
VOID* UefiLoadPEFile(CHAR16 *filename) {
    extern BOOLEAN k0_is_uefi;
    ASSERT(k0_is_uefi);

    // Open the file
    EFI_FILE_HANDLE fh;
    EFI_STATUS exec_file_result = ShellOpenFileByName(filename,
        &fh,
        EFI_FILE_MODE_READ,
        0);

    if (EFI_ERROR(exec_file_result)) {
        kernel_panic(L"Failed to open executable file %s: %r\n", filename, exec_file_result);
    }
    else if (k0_PRECONFIG_DEBUG) {
        Print(L"Opened %s\n", filename);
    }

    // Determine the file size
    EFI_FILE_INFO *exec_file_info = ShellGetFileInfo(fh);
    UINTN exec_file_pages = (exec_file_info->FileSize / EFI_PAGE_SIZE) + 1;

    // Allocate enough memory to hold the exec file
    CHAR8 *exec_buffer = AllocatePages(exec_file_pages);
    if (ZeroMem(exec_buffer, exec_file_pages * EFI_PAGE_SIZE) != exec_buffer) {
        kernel_panic(L"Unable to zero memory for executable image\n");
    }

    if (ISNULL(exec_buffer)) {
        kernel_panic(L"Failed to allocate pages to load exec image %s. Allocation operation returned NULL.\n", filename);
    }
    else if (k0_VERBOSE_DEBUG) {
        Print(L"Allocated %lu pages for %s image data\n", exec_file_pages, filename);
    }

    UINTN sz = exec_file_info->FileSize;
    EFI_STATUS exec_file_read_result = ShellReadFile(fh, &sz, exec_buffer);

    if (EFI_ERROR(exec_file_read_result)) {
        kernel_panic(L"Failed to read %s: %r\n", filename, exec_file_read_result);
    }
    else if (k0_VERBOSE_DEBUG) {
        Print(L"Read %s into memory\n", filename);
    }

    // Close the file
    EFI_STATUS efi_close_result = ShellCloseFile(&fh);
    if (EFI_ERROR(efi_close_result)) {
        kernel_panic(L"Unable to close %s\n", filename);
    }
    else if (k0_VERBOSE_DEBUG) {
        Print(L"Closed %s\n", filename);
    }

    // Make sure this is a PE file
    UINT32 *header_offset = (UINT32 *)(&exec_buffer[PE_HEADER_OFFSET]);
    coff_file_header *coff_fh = (coff_file_header *)(&exec_buffer[*header_offset]);

    if (coff_fh->signature != PE_SIGNATURE) {
        kernel_panic(L"PE image loaded is not a valid PE image: 0x%lx\n", coff_fh->signature);
    }
    
    // Ok, "valid" PE file at this point

    // Perform additional verification
    if (coff_fh->machine != PE_FILE_MACHINE_AMD64) {
        kernel_panic(L"PE image loaded is not for x64: 0x%lx\n", coff_fh->machine);
    }
    
    // Get the optional PE header, if it exists
    if (coff_fh->optional_header_size == 0) {
        kernel_panic(L"nebulae requires the optional PE header\n");
    }
    else {
        pe64_header *pe_fh = (pe64_header *)((UINT64)coff_fh + 0x18ULL);
        if (pe_fh->magic != PE_MAGIC_64) {
            Print(L"coff_fh @ 0x%lx\n", coff_fh);
            kernel_panic(L"PE image loaded is not 64-bit: pe_fh->magic @ 0x%lx == 0x%lx\n", &pe_fh->magic, pe_fh->magic);
        }

        // uefi executables have the Windows specific fields
        pe64_windows_fields *win_fields = (pe64_windows_fields *)((UINT64)pe_fh + 0x18ULL);

        if (win_fields->subsystem != PE_SUBSYSTEM_EFI_APPLICATION) {
            Print(L"win_fields @ 0x%lx\n", win_fields);
            kernel_panic(L"nebulae only supports applications of type efi_subsystem; this image is of type == 0x%lx\n", win_fields->subsystem);
        } 

        // docs state these data directories are optional;
        // but it doesn't make sense to have less than the
        // 16 every pe file seems to have (legacy?)
        // pe data directories - skip for now
        pe_data_dirs *data_dirs = (pe_data_dirs *)((UINT64)coff_fh + 0x70ULL);

        // The section headers come after the data directories
        pe_section_header *cur_pe_section_hdr = (pe_section_header *)((UINT64)data_dirs + (sizeof(pe_data_dir) * (UINT64)win_fields->number_of_data_dirs));

        // These sections need to be loaded into virtual memory somewhere
        // that is backed by physical memory
        UINTN i;
        for (i = 0; i < coff_fh->number_of_sections; i++) {

        }
    }
}
