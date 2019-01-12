// Copyright (c) 2018-2019 Nebulae Foundation. All rights reserved.
// Contains code Copyright (c) 2015  Finnbarr P. Murphy.   All rights reserved.
// Read more : https://blog.fpmurphy.com/2015/01/list-acpi-tables-from-uefi-shell.html
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

#include <Library/UefiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include "../../include/arch/uefi/kacpi.h"

#include "../../include/klib/kstring.h"

EFI_ACPI_SDT_HEADER *acpi_xsdt = NULL;

nebStatus ParseRSDP(EFI_ACPI_2_0_ROOT_SYSTEM_DESCRIPTION_POINTER *rsdp, CHAR16* guid) {
    EFI_ACPI_SDT_HEADER *xsdt, *entry;
    CHAR16 sig[20], oemstr[20];
    UINT32 entry_count;
    UINT64 *entry_ptr;
    UINT32 index;

    if (k0_VERBOSE_DEBUG) {
        Print(L"\n\nACPI GUID: %s\n", guid);
        kAscii2UnicodeStr((CHAR8 *)(rsdp->OemId), oemstr, 6);
        Print(L"\nFound RSDP. Version: %d  OEM ID: %s\n", (int)(rsdp->Revision), oemstr);
    }

    if (rsdp->Revision >= EFI_ACPI_2_0_ROOT_SYSTEM_DESCRIPTION_POINTER_REVISION) {
        xsdt = (EFI_ACPI_SDT_HEADER *)(rsdp->XsdtAddress);
    }
    else {
        if (k0_VERBOSE_DEBUG)
            Print(L"ERROR: No XSDT table found.\n");
        return NEBERROR_ACPI_XSDT_NOT_FOUND;
    }

    if (kStrnCmpA("XSDT", (CHAR8 *)(VOID *)(xsdt->Signature), 4)) {
        if (k0_VERBOSE_DEBUG)
            Print(L"ERROR: Invalid XSDT table found.\n");
        return NEBERROR_ACPI_INVALID_XSDT;
    }

    kAscii2UnicodeStr((CHAR8 *)(xsdt->OemId), oemstr, 6);
    entry_count = (xsdt->Length - sizeof(EFI_ACPI_SDT_HEADER)) / sizeof(UINT64);
    if (k0_VERBOSE_DEBUG)
        Print(L"Found XSDT @ 0x%lx. OEM ID: %s  Entry Count: %d\n\n", (UINT64)xsdt, oemstr, entry_count);

    entry_ptr = (UINT64 *)(xsdt + 1);
    acpi_xsdt = xsdt;
    for (index = 0; index < entry_count; index++, entry_ptr++) {
        entry = (EFI_ACPI_SDT_HEADER *)((UINTN)(*entry_ptr));
        kAscii2UnicodeStr((CHAR8 *)(entry->Signature), sig, 4);
        kAscii2UnicodeStr((CHAR8 *)(entry->OemId), oemstr, 6);
        if (k0_VERBOSE_DEBUG)
            Print(L"Found ACPI table: %s  Version: %d  OEM ID: %s\n", sig, (int)(entry->Revision), oemstr);
    }

    return NEB_OK;
}

nebStatus LocateACPI_XSDT() {

    EFI_CONFIGURATION_TABLE *ect = gST->ConfigurationTable;
    EFI_ACPI_2_0_ROOT_SYSTEM_DESCRIPTION_POINTER *rsdp = NULL;
    EFI_GUID acpi_table_guid = EFI_ACPI_TABLE_GUID;
    EFI_GUID acpi2_table_guid = EFI_ACPI_20_TABLE_GUID;
    CHAR16 guidstr[100];
    int index;

    // locate RSDP (Root System Description Pointer)
    for (index = 0; index < gST->NumberOfTableEntries; index++) {
        if ((CompareGuid(&(gST->ConfigurationTable[index].VendorGuid), &acpi_table_guid)) ||
            (CompareGuid(&(gST->ConfigurationTable[index].VendorGuid), &acpi2_table_guid))) {
            if (!kStrnCmpA("RSD PTR ", (CHAR8 *)(ect->VendorTable), 8)) {
                kGuid2String(guidstr, 100, &(gST->ConfigurationTable[index].VendorGuid));
                rsdp = (EFI_ACPI_2_0_ROOT_SYSTEM_DESCRIPTION_POINTER *)ect->VendorTable;
                ParseRSDP(rsdp, guidstr);
            }
        }
        ect++;
    }

    if (rsdp == NULL) { 
        if (k0_VERBOSE_DEBUG) {
            Print(L"ERROR: Could not find a RSDP.\n");
        }
        return NEBERROR_ACPI_RSDP_NOT_FOUND;
    }

    return NEB_OK;
}
