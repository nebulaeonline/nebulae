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

#include "../../include/arch/x64/local_apic.h"

UINT64 bsp_apic_address = NULL;
UINT8  bsp_apic_id = 0;
UINT32 *bsp_apic_version = NULL;

// Initialize the local APIC
VOID InitLocalAPIC() {
    if (!x64ReadCpuinfoFlags(X64_HAS_APIC)) {
        kernel_panic(L"nebulae requires APIC support on x64\n");
    }

    UINT64 apic_msr = AsmReadMsr64(X64_APIC_BASE_MSR);
    bsp_apic_address = apic_msr & X64_APIC_BASE_ADDR_MASK;

    if (!CHECK_BIT(apic_msr, X64_APIC_BSP)) {
        kernel_panic(L"nebulae is not running on the boot service processor\n");
    }

    bsp_apic_id = (UINT8)((cpu.cpuinfo[0x01].reg[X64_REG_EBX] & 0xFF000000) >> 24);
    if (k0_VERBOSE_DEBUG) {
        Print(L"Boot service processor APIC id: 0x%x\n", bsp_apic_id);
    }

    if (k0_VERBOSE_DEBUG && CHECK_BIT(apic_msr, X64_APIC_GLOBAL_ENABLE)) {
        Print(L"Global enable flag of APIC on boot service processor set\n");
    }

    bsp_apic_version = (UINT32*)(bsp_apic_address + X64_APIC_VERSION_REG_OFFSET);

    if (k0_VERBOSE_DEBUG) {
        Print(L"Boot service processor APIC version: 0x%x\n", *bsp_apic_version);
    }
}

// Read the given ioapic; if NULL, reads the bsp ioapic
UINT32 ReadIoAPIC(EFI_PHYSICAL_ADDRESS *ioapic_addr, UINT32 reg) {
    if (ISNULL(ioapic_addr)) {
        ioapic_addr = bsp_apic_address;
    }

    UINT32 volatile *ioapic = (UINT32 volatile *)ioapic_addr;
    ioapic[0] = (reg & 0xFF);
    return ioapic[4];
}

// Writes the given ioapic; if NULL, writes the bsp ioapic
VOID WriteIoAPIC(EFI_PHYSICAL_ADDRESS *ioapic_addr, UINT32 reg, UINT32 value) {
    if (ISNULL(ioapic_addr)) {
        ioapic_addr = bsp_apic_address;
    }

    UINT32 volatile *ioapic = (UINT32 volatile *)ioapic_addr;
    ioapic[0] = (reg & 0xFF);
    ioapic[4] = value;
}