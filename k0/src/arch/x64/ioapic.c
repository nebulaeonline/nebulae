// Copyright (c) 2005-2019 Nebulae Foundation. All rights reserved.
// Portions Copyright (c) 2012 Patrick Doane and others.  See 
//   AUTHORS.x64_ioapic.txt file for list.
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

#include "../../include/arch/x64/ioapic.h"
#include "../../include/arch/x64/mmio.h"

UINT64 bsp_apic_addr = NULL;
UINT8  bsp_apic_id = 0;
UINT32 *bsp_apic_version = NULL;

// Initialize the legacy PIC
VOID InitPIC() {
    // ICW1: start initialization, ICW4 needed
    IoWrite8(X64_PIC1_CMD, X64_PIC_ICW1_INIT | X64_PIC_ICW1_ICW4);
    IoWrite8(X64_PIC2_CMD, X64_PIC_ICW1_INIT | X64_PIC_ICW1_ICW4);

    // ICW2: interrupt vector address
    IoWrite8(X64_PIC1_DATA, X64_PIC_IRQ_BASE);
    IoWrite8(X64_PIC2_DATA, X64_PIC_IRQ_BASE + 8);

    // ICW3: master/slave wiring
    IoWrite8(X64_PIC1_DATA, 4);
    IoWrite8(X64_PIC2_DATA, 2);

    // ICW4: 8086 mode, not special fully nested, not buffered, normal EOI
    IoWrite8(X64_PIC1_DATA, X64_PIC_ICW4_8086);
    IoWrite8(X64_PIC2_DATA, X64_PIC_ICW4_8086);

    // OCW1: Disable all IRQs
    IoWrite8(X64_PIC1_DATA, 0xff);
    IoWrite8(X64_PIC2_DATA, 0xff);
}

// Initialize the I/O APIC
VOID InitLocalAPIC() {
    if (!x64ReadCpuinfoFlags(X64_HAS_APIC)) {
        kernel_panic(L"nebulae requires IOAPIC support on x64\n");
    }

    // This SHOULD NOT be hardcoded, and should instead be located
    // within the ACPI tables.  Due to dependencies at this stage
    // of development, this will stay as-is for a bit longer.
    // #TODO #FIXME #APIC #IOAPIC #HARDCODED
    bsp_apic_addr = 0xFEC00000;
}

// Read the given ioapic; if NULL, reads the bsp ioapic
UINT32 ReadIOApic(EFI_PHYSICAL_ADDRESS *ioapic_addr, UINT8 reg) {
    if (ISNULL(ioapic_addr)) {
        ioapic_addr = bsp_apic_addr;
    }

    MmioWrite32(ioapic_addr + X64_APIC_IOREGSEL, reg);
    return MmioRead32(ioapic_addr + X64_APIC_IOWIN);
}

// Writes the given ioapic; if NULL, writes the bsp ioapic
VOID WriteIOApic(EFI_PHYSICAL_ADDRESS *ioapic_addr, UINT8 reg, UINT32 value) {
    if (ISNULL(ioapic_addr)) {
        ioapic_addr = bsp_apic_addr;
    }

    MmioWrite32(bsp_apic_addr + X64_APIC_IOREGSEL, reg);
    MmioWrite32(bsp_apic_addr + X64_APIC_IOWIN, value);
}
