// Copyright (c) 2005-2019 Nebulae Foundation. All rights reserved.
// Contains code Copyright (c) 2015  Finnbarr P. Murphy.   All rights reserved.
// Read more : https://blog.fpmurphy.com/2015/01/list-acpi-tables-from-uefi-shell.html
// Contains code Copyright (c) 2012 Patrick Doane and others.  See 
//   AUTHORS.x64_acpi.txt file for list.
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

#include <Library/UefiLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UefiBootServicesTableLib.h>

#include "../../include/arch/x64/acpi.h"
#include "../../include/arch/x64/mmio.h"
#include "../../include/arch/uefi/memory.h"

#include "../../include/klib/kstring.h"

BOOLEAN _xsdt_located = FALSE;
extern nebulae_system_table *system_table;

nebStatus ParseRSDP(X64_ACPI_RSDP *rsdp, CHAR16* guid) {
    EFI_ACPI_DESCRIPTION_HEADER *xsdt, *entry;
    CHAR16 sig[20], oemstr[20];
    UINT32 entry_count;
    UINT64 *entry_ptr;
    UINT32 index;

    if (k0_VERBOSE_DEBUG) {
        Print(L"\n\nACPI GUID: %s\n", guid);
        kAscii2UnicodeStr((CHAR8 *)(rsdp->OemId), oemstr, 6);
        Print(L"\nFound Rsdp. Version: %d  OEM ID: %s\n", (int)(rsdp->Revision), oemstr);
    }

    if (rsdp->Revision >= X64_ACPI_RSDP_REV) {
        xsdt = (EFI_ACPI_DESCRIPTION_HEADER *)(rsdp->XsdtAddress);
    }
    else {
        if (k0_VERBOSE_DEBUG) {
            Print(L"ERROR: No Xsdt table found.\n");
        }
        return NEBERROR_ACPI_XSDT_NOT_FOUND;
    }

    if (xsdt->Signature & 0xFFFFFFFF00 != 0x5444535800) {
        if (k0_VERBOSE_DEBUG) {
            Print(L"ERROR: Invalid Xsdt table found: %0x%x\n", xsdt->Signature);
        }
        return NEBERROR_ACPI_INVALID_XSDT;
    }
        
    entry_count = (xsdt->Length - sizeof(EFI_ACPI_DESCRIPTION_HEADER)) / sizeof(UINT64);
    if (k0_VERBOSE_DEBUG) {
        kAscii2UnicodeStr((CHAR8 *)(xsdt->OemId), oemstr, 6);
        Print(L"Found Xsdt @ 0x%lx. OEM ID: %s  Entry Count: %d\n\n", (UINT64)xsdt, oemstr, entry_count);
    }

    // Xsdt has been located
    extern preboot_mem_block k0_kernel_system_area;

    // Put a pointer to the xsdt into the system table
    system_table->acpi_xsdt = xsdt;
    
    // Set up the acpi cpu table header
    system_table->acpi_cpu_table.signature = NEBULAE_ACPI_CPU_TABLE;
    system_table->acpi_cpu_table.type_id = NEBTYPE_ACPI_CPU;
    system_table->acpi_cpu_table.struct_sz = sizeof(nebulae_sys_element);
    system_table->acpi_cpu_table.struct_count = 0;
    system_table->acpi_cpu_table.element0_addr = NULL;
    system_table->acpi_cpu_table.elementn_addr = NULL;
    system_table->acpi_cpu_table.parent_addr = NULL;
    system_table->acpi_cpu_table.reserved = NEBULAE_ACPI_CPU_TABLE;

    // Set up the acpi interrupt override header
    system_table->acpi_interrupt_override_table.signature = NEBULAE_ACPI_INTERRUPT_OVERRIDE_TABLE;
    system_table->acpi_interrupt_override_table.type_id = NEBTYPE_ACPI_OVERRIDDEN_INTERRUPT;
    system_table->acpi_interrupt_override_table.struct_sz = sizeof(nebulae_sys_element);
    system_table->acpi_interrupt_override_table.struct_count = 0;
    system_table->acpi_interrupt_override_table.element0_addr = NULL;
    system_table->acpi_interrupt_override_table.elementn_addr = NULL;
    system_table->acpi_interrupt_override_table.parent_addr = NULL;
    system_table->acpi_interrupt_override_table.reserved = NEBULAE_ACPI_INTERRUPT_OVERRIDE_TABLE;

    // Let others in preboot know we're good with Xsdt
    _xsdt_located = TRUE;

    // Read the ACPI tables
    entry_ptr = (UINT64 *)(xsdt + 1);
    X64_MADT_HEADER *madt = NULL;

    for (index = 0; index < entry_count; index++, entry_ptr++) {
        entry = (EFI_ACPI_DESCRIPTION_HEADER *)((UINTN)(*entry_ptr));
        
        switch (entry->Signature) {
        case ACPI_FACP_TABLE_SIG:
            break;
        case ACPI_MADT_TABLE_SIG:
            madt = entry;
            system_table->lapic_base_addr = madt->LocalApicAddress;

            UINT8 *xsdt_curr = (UINT8 *)(madt + 1);
            UINT8 *xsdt_end = (UINT8 *)madt + (madt->Header.Length);

            while (xsdt_curr < xsdt_end) {
                x64_apic_common_header *hdr = (x64_apic_common_header *)xsdt_curr;
                UINT8 type = hdr->type;
                UINT8 length = hdr->length;
                x64_local_apic *la = NULL;
                x64_io_apic *ioapic = NULL;
                x64_overriden_interrupt *oi = NULL;

                switch (type) {
                case EFI_ACPI_1_0_PROCESSOR_LOCAL_APIC:
                    la = hdr;
                    system_table->cpu_count++;
                    nebulae_sys_element *new_cpu = NULL;

                    if (ISNULL(system_table->acpi_cpu_table.element0_addr)) {
                        system_table->acpi_cpu_table.element0_addr = kPrebootCriticalMalloc(
                            &k0_kernel_system_area,
                            sizeof(nebulae_sys_element),
                            ALIGN_16);
                        system_table->acpi_cpu_table.elementn_addr =
                            system_table->acpi_cpu_table.element0_addr;                        
                    }
                    else {
                        system_table->acpi_cpu_table.elementn_addr =
                            kPrebootCriticalMalloc(
                                &k0_kernel_system_area,
                                sizeof(nebulae_sys_element),
                                ALIGN_16);
                    }
                    system_table->acpi_cpu_table.struct_count++;

                    new_cpu = (nebulae_sys_element *)(system_table->acpi_cpu_table.elementn_addr);
                    new_cpu->type_id = NEBTYPE_ACPI_CPU;
                    new_cpu->value = (UINT64)la;
                    break;
                case EFI_ACPI_1_0_IO_APIC:
                    ioapic = hdr;
                    system_table->ioapic_base_addr = ioapic->addr;
                    break;
                case EFI_ACPI_1_0_INTERRUPT_SOURCE_OVERRIDE:
                    oi = hdr;
                    nebulae_sys_element *new_oi = NULL;

                    if (ISNULL(system_table->acpi_interrupt_override_table.element0_addr)) {
                        system_table->acpi_interrupt_override_table.element0_addr = kPrebootCriticalMalloc(
                            &k0_kernel_system_area,
                            sizeof(nebulae_sys_element),
                            ALIGN_16);
                        system_table->acpi_interrupt_override_table.elementn_addr =
                            system_table->acpi_interrupt_override_table.element0_addr;
                    }
                    else {
                        system_table->acpi_interrupt_override_table.elementn_addr =
                            kPrebootCriticalMalloc(
                                &k0_kernel_system_area,
                                sizeof(nebulae_sys_element),
                                ALIGN_16);
                    }
                    system_table->acpi_interrupt_override_table.struct_count++;

                    new_oi = (nebulae_sys_element *)(system_table->acpi_interrupt_override_table.elementn_addr);
                    new_oi->type_id = NEBTYPE_ACPI_OVERRIDDEN_INTERRUPT;
                    new_oi->value = (UINT64)oi;
                    break;
                case EFI_ACPI_1_0_NON_MASKABLE_INTERRUPT_SOURCE:
                    break;
                case EFI_ACPI_1_0_LOCAL_APIC_NMI:
                    break;
                }

                xsdt_curr += length;
            }
            break;
        case ACPI_SSDT_TABLE_SIG:
            break;
        case ACPI_HPET_TABLE_SIG:
            break;
        case ACPI_MCFG_TABLE_SIG:
            break;
        case ACPI_BGRT_TABLE_SIG:
            break;
        }

        if (k0_VERBOSE_DEBUG) {
            kAscii2UnicodeStr((CHAR8 *)(entry->OemId), oemstr, 6);
            Print(L"Found ACPI table: 0x%lx  Version: %d  OEM ID: %s\n", entry->Signature, (int)(entry->Revision), oemstr);
        }
    }
    //kernel_panic(L"");

    return NEB_OK;
}

nebStatus x64PreInitAcpi() {

    EFI_CONFIGURATION_TABLE *ect = gST->ConfigurationTable;
    EFI_ACPI_6_2_ROOT_SYSTEM_DESCRIPTION_POINTER *rsdp = NULL;
    EFI_GUID acpi_table_guid = EFI_ACPI_10_TABLE_GUID;
    EFI_GUID acpi2_table_guid = EFI_ACPI_TABLE_GUID;
    CHAR16 guidstr[100];
    int index;

    // locate RSDP (Root System Description Pointer)
    for (index = 0; index < gST->NumberOfTableEntries; index++) {
        if ((CompareGuid(&(gST->ConfigurationTable[index].VendorGuid), &acpi_table_guid)) ||
            (CompareGuid(&(gST->ConfigurationTable[index].VendorGuid), &acpi2_table_guid))) {
            if (!kStrnCmpA("RSD PTR ", (CHAR8 *)(ect->VendorTable), 8)) {
                kGuid2String(guidstr, 100, &(gST->ConfigurationTable[index].VendorGuid));
                rsdp = (EFI_ACPI_6_2_ROOT_SYSTEM_DESCRIPTION_POINTER *)ect->VendorTable;
                ParseRSDP(rsdp, guidstr);
            }
        }
        ect++;
    }

    if (ISNULL(rsdp)) { 
        if (k0_VERBOSE_DEBUG) {
            Print(L"ERROR: Could not find an RSDP.\n");
        }
        return NEBERROR_ACPI_RSDP_NOT_FOUND;
    }

    return NEB_OK;
}

// Initialize the legacy PIC
VOID x64InitPIC() {
    // ICW1: start initialization, ICW4 needed
    MmioWrite8(X64_PIC1_CMD, X64_PIC_ICW1_INIT | X64_PIC_ICW1_ICW4);
    MmioWrite8(X64_PIC2_CMD, X64_PIC_ICW1_INIT | X64_PIC_ICW1_ICW4);

    // ICW2: interrupt vector address
    MmioWrite8(X64_PIC1_DATA, X64_PIC_IRQ_BASE);
    MmioWrite8(X64_PIC2_DATA, X64_PIC_IRQ_BASE + 8);

    // ICW3: master/slave wiring
    MmioWrite8(X64_PIC1_DATA, 4);
    MmioWrite8(X64_PIC2_DATA, 2);

    // ICW4: 8086 mode, not special fully nested, not buffered, normal EOI
    MmioWrite8(X64_PIC1_DATA, X64_PIC_ICW4_8086);
    MmioWrite8(X64_PIC2_DATA, X64_PIC_ICW4_8086);

    // OCW1: Disable all IRQs
    MmioWrite8(X64_PIC1_DATA, 0xff);
    MmioWrite8(X64_PIC2_DATA, 0xff);
}

// Initialize the I/O APIC
VOID x64InitIoApic() {
    if (!x64ReadCpuinfoFlags(X64_HAS_APIC)) {
        kernel_panic(L"nebulae requires IOAPIC support on x64\n");
    }
    
    if (ISNULL(system_table->ioapic_base_addr)) {
        kernel_panic(L"Fatal attempt to initialize IOAPIC before ACPI\n");
    }

    // Get # of entries supported by IOAPIC
    UINT32 x = x64ReadApic(NULL, X64_APIC_VERSION_REG_OFFSET);
    UINT32 count = ((x >> 16) & 0xFF) + 1;

    UINT32 i;
    for (i = 1; i < count; i++) {
        x64SetIoApicEntry(i, BIT16_64);
    }
}

// Initialize the local APIC
VOID x64InitLocalApic() {

    // Clear TPR to enable all interrupts
    x64WriteApic(system_table->lapic_base_addr, X64_APIC_TPR_OFFSET, 0);

    // Flat mode, logical id 1 #TODO #FIXME
    x64WriteApic(system_table->lapic_base_addr, X64_APIC_DEST_FORMAT_REG_OFFSET, 0xFFFFFFFF);
    x64WriteApic(system_table->lapic_base_addr, X64_APIC_LOGICAL_DEST_REG_OFFSET, 0x01000000);

    // Set bit in spurious interrupt vector
    x64WriteApic(system_table->lapic_base_addr, X64_APIC_SPRIOUS_INT_VECTOR_OFFSET, 0x100 | 0xFF);
}

// Set an entry in the IOAPIC
VOID x64SetIoApicEntry(UINT8 index, UINT64 data) {
    x64WriteApic(NULL, X64_APIC_IOREDTBL + index * 2, LO32(data));
    x64WriteApic(NULL, X64_APIC_IOREDTBL + index * 2 + 1, HI32(data));
}

// Read the given apic; if NULL, reads the ioapic
UINT32 x64ReadApic(EFI_PHYSICAL_ADDRESS *apic_addr, UINT8 reg) {
    if (ISNULL(apic_addr)) {
        apic_addr = system_table->ioapic_base_addr;
    }

    MmioWrite32(apic_addr + X64_APIC_IOREGSEL, reg);
    return MmioRead32(apic_addr + X64_APIC_IOWIN);
}

// Writes the given apic; if NULL, writes the ioapic
VOID x64WriteApic(EFI_PHYSICAL_ADDRESS *apic_addr, UINT8 reg, UINT32 value) {
    if (ISNULL(apic_addr)) {
        apic_addr = system_table->ioapic_base_addr;
    }

    MmioWrite32(apic_addr + X64_APIC_IOREGSEL, reg);
    MmioWrite32(apic_addr + X64_APIC_IOWIN, value);
}
