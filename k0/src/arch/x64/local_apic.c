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

#include "../../include/arch/x64/local_apic.h"
#include "../../include/arch/x64/mmio.h"

#define IOREGSEL                    0x00
#define IOWIN                       0x10
#define IOAPICID                    0x00
#define IOAPICVER                   0x01
#define IOAPICARB                   0x02
#define IOREDTBL                    0x10

UINT64 bsp_apic_addr = NULL;
UINT8  bsp_apic_id = 0;
UINT32 *bsp_apic_version = NULL;

// #TODO THIS NEEDS FIXING ASAP.... NOT CORRECT.
// Initialize the local APIC
VOID InitLocalAPIC() {
    if (!x64ReadCpuinfoFlags(X64_HAS_APIC)) {
        kernel_panic(L"nebulae requires APIC support on x64\n");
    }

    // For Local APIC
    // UINT64 apic_msr = AsmReadMsr64(X64_APIC_BASE_MSR);
    // bsp_apic_addr = apic_msr & X64_APIC_BASE_ADDR_MASK;
    bsp_apic_addr = 0xFEC00000;

    /*
    if (!CHECK_BIT(apic_msr, X64_APIC_BSP)) {
        kernel_panic(L"nebulae is not running on the boot service processor\n");
    }
    */
}

// Read the given ioapic; if NULL, reads the bsp ioapic
UINT32 ReadIOApic(EFI_PHYSICAL_ADDRESS *ioapic_addr, UINT8 reg) {
    if (ISNULL(ioapic_addr)) {
        ioapic_addr = bsp_apic_addr;
    }

    MmioWrite32(ioapic_addr + IOREGSEL, reg);
    return MmioRead32(ioapic_addr + IOWIN);
}

// Writes the given ioapic; if NULL, writes the bsp ioapic
VOID WriteIOApic(EFI_PHYSICAL_ADDRESS *ioapic_addr, UINT8 reg, UINT32 value) {
    if (ISNULL(ioapic_addr)) {
        ioapic_addr = bsp_apic_addr;
    }

    MmioWrite32(bsp_apic_addr + IOREGSEL, reg);
    MmioWrite32(bsp_apic_addr + IOWIN, value);
}
