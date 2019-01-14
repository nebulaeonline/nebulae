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

// Compiler Builtins
const builtin = @import("builtin");

const baselib = @cImport({
    @cInclude("Base.h");
    @cInclude("Library/BaseLib.h");
});

// CPU struct
pub const 
pub const x86cpu = struct {
    PhysicalAddressBits: u8,
    LinearAddressBits: u8,
    MaxBasicCpuidEAX: u32,
    MaxExtendedCpuidEAX: u32,
    VendorString: [13]u8,
    Model: u8,
    ExtModel: u8,
    Family: u8,
    ExtFamily: u8,
    Stepping: u8,
    BrandIndex: u8,
    ClflushLineSize: u8,
    CacheLineSize: u32,
    MaxAPICIds: u8,
    InitialAPICId: u8,
    SSE3: bool,
    PCLMULQDQ: bool,
    DTES64: bool,
    MONITOR: bool,
    CPLDebugStore: bool,
    VMX: bool,
    SMX: bool,
    SpeedStep: bool,
    ThermalMonitor2: bool,
    SSSE3: bool,
    L1ContextId: bool,
    SiliconDebug: bool,
    FMA: bool,
    CMPXCHG16B: bool,
    TPRUpdateControl: bool,
    PDCM: bool,
    PCID: bool,
    DCA: bool,
    SSE41: bool,
    SSE42: bool,
    X2APIC: bool,
    MOVBE: bool,
    POPCNT: bool,
    TSCDeadline: bool,
    AESNI: bool,
    XSAVE: bool,
    OSXSAVE: bool,
    AVX: bool,
    F16C: bool,
    RDRAND: bool,
    Fpu: bool,
    V8086: bool,
    Debug: bool,
    PSE: bool,
    RDTSC: bool,
    MSR: bool,
    PAE: bool,
    MachineCheckException: bool,
    CMPXCHG8B: bool,
    APIC: bool,
    SYSENTER: bool,
    MTRR: bool,
    PGE: bool,
    MachineCheckArchitecture: bool,
    CMOV: bool,
    PAT: bool,
    PSE36: bool,
    PSN: bool,
    CLFLUSH: bool,
    DebugStore: bool,
    ACPI: bool,
    MMX: bool,
    FXSAVE: bool,
    SSE: bool,
    SSE2: bool,
    SelfSnoop: bool,
    HyperThreading: bool,
    ThermalMonitor: bool,
    PBE: bool,
};

// Fill out a cpu information struct
pub fn IdentifyCPU(cpu_struct: *x86cpu) void {
    var EAX: u32 = 0;
    var EBX: u32 = 0;
    var ECX: u32 = 0;
    var EDX: u32 = 0;
    var retval: u32 = 0;
    
    // Start at the beginning - EAX == 00H
    retval = baselib.AsmCpuid(0x00, @ptrCast(?[*]c_uint, &EAX), @ptrCast(?[*]c_uint, &EBX), @ptrCast(?[*]c_uint, &ECX), @ptrCast(?[*]c_uint, &EDX));
    cpu_struct.MaxBasicCpuidEAX = EAX;
    cpu_struct.VendorString[0] = @truncate(u8, ((EBX & 0xFF000000) >> 24));
    cpu_struct.VendorString[1] = @truncate(u8, ((EBX & 0x00FF0000) >> 16));
    cpu_struct.VendorString[2] = @truncate(u8, ((EBX & 0x0000FF00) >> 8));
    cpu_struct.VendorString[3] = @truncate(u8, (EBX & 0x000000FF));
    cpu_struct.VendorString[4] = @truncate(u8, ((ECX & 0xFF000000) >> 24));
    cpu_struct.VendorString[5] = @truncate(u8, ((ECX & 0x00FF0000) >> 16));
    cpu_struct.VendorString[6] = @truncate(u8, ((ECX & 0x0000FF00) >> 8));
    cpu_struct.VendorString[7] = @truncate(u8, (ECX & 0x000000FF));
    cpu_struct.VendorString[8] = @truncate(u8, ((EDX & 0xFF000000) >> 24));
    cpu_struct.VendorString[9] = @truncate(u8, ((EDX & 0x00FF0000) >> 16));
    cpu_struct.VendorString[10] = @truncate(u8, ((EDX & 0x0000FF00) >> 8));
    cpu_struct.VendorString[11] = @truncate(u8, (EDX & 0x000000FF));
    cpu_struct.VendorString[12] = '\x00';

    // EAX == 01H
    if (cpu_struct.MaxBasicCpuidEAX >= 0x01) {        
        retval = baselib.AsmCpuid(0x01, @ptrCast(?[*]c_uint, &EAX), @ptrCast(?[*]c_uint, &EBX), @ptrCast(?[*]c_uint, &ECX), @ptrCast(?[*]c_uint, &EDX));
        cpu_struct.Model = @truncate(u8, ((EAX & 0x000000F0) >> 4));
        cpu_struct.ExtModel = @truncate(u8, ((EAX & 0x000F0000) >> 16));
        cpu_struct.Family = @truncate(u8, ((EAX & 0x00000F00) >> 8));
        cpu_struct.ExtFamily = @truncate(u8, ((EAX & 0x0FF00000) >> 20));
        cpu_struct.Stepping = @truncate(u8, (EAX & 0x0000000F));
        cpu_struct.BrandIndex = @truncate(u8, (EBX & 0x000000FF));
        cpu_struct.ClflushLineSize = @truncate(u8, ((EBX & 0x0000FF00) >> 8));
        cpu_struct.CacheLineSize = cpu_struct.ClflushLineSize * 8;
        cpu_struct.MaxAPICIds = @truncate(u8, ((EBX & 0x00FF0000) >> 16));
        cpu_struct.InitialAPICId = @truncate(u8, ((EBX & 0xFF000000) >> 24));

        // Bit 0 ECX - SSE3 Support
        if ((ECX & (1 << 0)) == (1 << 0)) {
            cpu_struct.SSE3 = true;
        }

        // Bit 1 ECX - PCLMULQDQ Support
        if ((ECX & (1 << 1)) == (1 << 1)) {
            cpu_struct.PCLMULQDQ = true;
        }

        // Bit 2 ECX - DTES64 Support
        if ((ECX & (1 << 2)) == (1 << 2)) {
            cpu_struct.DTES64 = true;
        }

        // Bit 3 ECX - MONITOR Support
        if ((ECX & (1 << 3)) == (1 << 3)) {
            cpu_struct.MONITOR = true;
        }

        // Bit 4 ECX - CPLDebugStore Support
        if ((ECX & (1 << 4)) == (1 << 4)) {
            cpu_struct.CPLDebugStore = true;
        }

        // Bit 5 ECX - VMX Support
        if ((ECX & (1 << 5)) == (1 << 5)) {
            cpu_struct.VMX = true;
        }

        // Bit 6 ECX - SMX Support
        if ((ECX & (1 << 6)) == (1 << 6)) {
            cpu_struct.SMX = true;
        }

        // Bit 7 ECX - SpeedStep Support
        if ((ECX & (1 << 7)) == (1 << 7)) {
            cpu_struct.SpeedStep = true;
        }

        // Bit 8 ECX - ThermalMonitor2 Support
        if ((ECX & (1 << 8)) == (1 << 8)) {
            cpu_struct.ThermalMonitor2 = true;
        }

        // Bit 9 ECX - SSSE3 Support
        if ((ECX & (1 << 9)) == (1 << 9)) {
            cpu_struct.SSSE3 = true;
        }

        // Bit 10 ECX - L1ContextId Support
        if ((ECX & (1 << 10)) == (1 << 10)) {
            cpu_struct.L1ContextId = true;
        }

        // Bit 11 ECX - SiliconDebug Support
        if ((ECX & (1 << 11)) == (1 << 11)) {
            cpu_struct.SiliconDebug = true;
        }

        // Bit 12 ECX - FMA Support
        if ((ECX & (1 << 12)) == (1 << 12)) {
            cpu_struct.FMA = true;
        }

        // Bit 13 ECX - CMPXCHG16B Support
        if ((ECX & (1 << 13)) == (1 << 13)) {
            cpu_struct.CMPXCHG16B = true;
        }

        // Bit 14 ECX - TPRUpdateControl Support
        if ((ECX & (1 << 14)) == (1 << 14)) {
            cpu_struct.TPRUpdateControl = true;
        }

        // Bit 15 ECX - PDCM Support
        if ((ECX & (1 << 15)) == (1 << 15)) {
            cpu_struct.PDCM = true;
        }

        // Bit 16 ECX - Reserved

        // Bit 17 ECX - PCID Support
        if ((ECX & (1 << 17)) == (1 << 17)) {
            cpu_struct.PCID = true;
        }

        // Bit 18 ECX - DCA Support
        if ((ECX & (1 << 18)) == (1 << 18)) {
            cpu_struct.DCA = true;
        }

        // Bit 19 ECX - SSE41 Support
        if ((ECX & (1 << 19)) == (1 << 19)) {
            cpu_struct.SSE41 = true;
        }

        // Bit 20 ECX - SSE42 Support
        if ((ECX & (1 << 20)) == (1 << 20)) {
            cpu_struct.SSE42 = true;
        }

        // Bit 21 ECX - X2APIC Support
        if ((ECX & (1 << 21)) == (1 << 21)) {
            cpu_struct.X2APIC = true;
        }

        // Bit 22 ECX - MOVBE Support
        if ((ECX & (1 << 22)) == (1 << 22)) {
            cpu_struct.MOVBE = true;
        }

        // Bit 23 ECX - POPCNT Support
        if ((ECX & (1 << 23)) == (1 << 23)) {
            cpu_struct.POPCNT = true;
        }

        // Bit 24 ECX - TSCDeadline Support
        if ((ECX & (1 << 24)) == (1 << 24)) {
            cpu_struct.TSCDeadline = true;
        }

        // Bit 25 ECX - AESNI Support
        if ((ECX & (1 << 25)) == (1 << 25)) {
            cpu_struct.AESNI = true;
        }

        // Bit 26 ECX - XSAVE Support
        if ((ECX & (1 << 26)) == (1 << 26)) {
            cpu_struct.XSAVE = true;
        }

        // Bit 27 ECX - OSXSAVE Support
        if ((ECX & (1 << 27)) == (1 << 27)) {
            cpu_struct.OSXSAVE = true;
        }

        // Bit 28 ECX - AVX Support
        if ((ECX & (1 << 28)) == (1 << 28)) {
            cpu_struct.AVX = true;
        }

        // Bit 29 ECX - F16C Support
        if ((ECX & (1 << 29)) == (1 << 29)) {
            cpu_struct.F16C = true;
        }

        // Bit 30 ECX - RDRAND Support
        if ((ECX & (1 << 30)) == (1 << 30)) {
            cpu_struct.RDRAND = true;
        }

        // Bit 31 ECX == 0

        // Bit 0 EDX - x87 FPU Support
        if ((EDX & (1 << 0)) == (1 << 0)) {
            cpu_struct.Fpu = true;
        }

        // Bit 1 EDX - Virtual 8086 Support
        if ((EDX & (1 << 1)) == (1 << 1)) {
            cpu_struct.V8086 = true;
        }

        // Bit 2 EDX - Debug Extensions Support
        if ((EDX & (1 << 2)) == (1 << 2)) {
            cpu_struct.Debug = true;
        }

        // Bit 3 EDX - Page Size Extensions Support
        if ((EDX & (1 << 3)) == (1 << 3)) {
            cpu_struct.PSE = true;
        }

        // Bit 4 EDX - Time Stamp Counter Support
        if ((EDX & (1 << 4)) == (1 << 4)) {
            cpu_struct.RDTSC = true;
        }

        // Bit 5 EDX - MSR Support
        if ((EDX & (1 << 5)) == (1 << 5)) {
            cpu_struct.MSR = true;
        }

        // Bit 6 EDX - PAE Support
        if ((EDX & (1 << 6)) == (1 << 6)) {
            cpu_struct.PAE = true;
        }

        // Bit 7 EDX - Machine Check Exception Support
        if ((EDX & (1 << 7)) == (1 << 7)) {
            cpu_struct.MachineCheckException = true;
        }

        // Bit 8 EDX - CMPXCHG8B Support
        if ((EDX & (1 << 8)) == (1 << 8)) {
            cpu_struct.CMPXCHG8B = true;
        }

        // Bit 9 EDX - APIC Support
        if ((EDX & (1 << 9)) == (1 << 9)) {
            cpu_struct.APIC = true;
        }

        // Bit 10 EDX - Reserved

        // Bit 11 EDX - SYSENTER Support
        if ((EDX & (1 << 11)) == (1 << 11)) {
            cpu_struct.SYSENTER = true;
        }

        // Bit 12 EDX - MTRR Support
        if ((EDX & (1 << 12)) == (1 << 12)) {
            cpu_struct.MTRR = true;
        }

        // Bit 13 EDX - PGE Support
        if ((EDX & (1 << 13)) == (1 << 13)) {
            cpu_struct.PGE = true;
        }

        // Bit 14 EDX - Machine Check Architecture Support
        if ((EDX & (1 << 14)) == (1 << 14)) {
            cpu_struct.MachineCheckArchitecture = true;
        }

        // Bit 15 EDX - CMOV Support
        if ((EDX & (1 << 15)) == (1 << 15)) {
            cpu_struct.CMOV = true;
        }

        // Bit 16 EDX - PAT
        if ((EDX & (1 << 16)) == (1 << 16)) {
            cpu_struct.PAT = true;
        }

        // Bit 17 EDX - PSE36 Support
        if ((EDX & (1 << 17)) == (1 << 17)) {
            cpu_struct.PSE36 = true;
        }

        // Bit 18 EDX - Processor Serial (PSN) Support
        if ((EDX & (1 << 18)) == (1 << 18)) {
            cpu_struct.PSN = true;
        }

        // Bit 19 EDX - CLFLUSH Support
        if ((EDX & (1 << 19)) == (1 << 19)) {
            cpu_struct.CLFLUSH = true;
        }

        // Bit 20 EDX - Reserved
        
        // Bit 21 EDX - DebugStore Support
        if ((EDX & (1 << 21)) == (1 << 21)) {
            cpu_struct.DebugStore = true;
        }

        // Bit 22 EDX - ACPI Support
        if ((EDX & (1 << 22)) == (1 << 22)) {
            cpu_struct.ACPI = true;
        }

        // Bit 23 EDX - MMX Support
        if ((EDX & (1 << 23)) == (1 << 23)) {
            cpu_struct.MMX = true;
        }

        // Bit 24 EDX - FXSAVE Support
        if ((EDX & (1 << 24)) == (1 << 24)) {
            cpu_struct.FXSAVE = true;
        }

        // Bit 25 EDX - SSE Support
        if ((EDX & (1 << 25)) == (1 << 25)) {
            cpu_struct.SSE = true;
        }

        // Bit 26 EDX - SSE2 Support
        if ((EDX & (1 << 26)) == (1 << 26)) {
            cpu_struct.SSE2 = true;
        }

        // Bit 27 EDX - Self Snoop Support
        if ((EDX & (1 << 27)) == (1 << 27)) {
            cpu_struct.SelfSnoop = true;
        }

        // Bit 28 EDX - HyperThreading Support
        if ((EDX & (1 << 28)) == (1 << 28)) {
            cpu_struct.HyperThreading = true;
        }

        // Bit 29 EDX - ThermalMonitor Support
        if ((EDX & (1 << 29)) == (1 << 29)) {
            cpu_struct.ThermalMonitor = true;
        }

        // Bit 30 EDX - Reserved
        
        // Bit 31 EDX - PBE Support
        if ((EDX & (1 << 31)) == (1 << 31)) {
            cpu_struct.PBE = true;
        }
    }

    // This cpuid function returns the maximum supported EAX value supported
    // by this cpu's cpuid function (hmmm...)
    retval = baselib.AsmCpuid(0x80000000, @ptrCast(?[*]c_uint, &EAX), null, null, null);
    if (EAX < 0x80000008) {
        @panic("Unable to determine CPU bit support (hardware & logical)");
    }
    
    retval = baselib.AsmCpuid(0x80000008, @ptrCast(?[*]c_uint, &EAX), null, null, null);

    cpu_struct.PhysicalAddressBits = @truncate(u8, (EAX & 0x000000FF));
    cpu_struct.LinearAddressBits = @truncate(u8, ((EAX & 0x0000FF00) >> 8));
}

pub var cpu0: x86cpu = undefined;

// Function to query the processor to determine the physical and linear
// address bits supported
pub fn InitCPU() void {
    IdentifyCPU(&cpu0);
}

// x86 Segment Selector
// 16-bits / 2 bytes
// data 00:1  - RPL
// data 02    - Table Indicator (clear == GDT / set == LDT)
// data 03:15 - Index into GDT or LDT
pub const SegmentSelector = u16; 

pub const SEG_SEL_RPL0: u8 = 0x00;
pub const SEG_SEL_RPL1: u8 = 0x01;
pub const SEG_SEL_RPL2: u8 = 0x02;
pub const SEG_SEL_RPL3: u8 = 0x03;
pub const SEG_SEL_LDT: u8 = 0x04;
pub const SEG_SEL_GDT: u8 = 0x00;

pub fn PrepareSegmentSelector(sel: *SegmentSelector,
    RequestedPrivilegeLevel: u8,
    TableIndicator: u8,
    Index: u16) bool {
    
    if (RequestedPrivilegeLevel > 3) {
        return false;
    }

    if (TableIndicator != SEG_SEL_LDT and TableIndicator != SEG_SEL_GDT) {
        return false;
    }

    if (Index >= (1 << 13)) {
        return false;
    }

    sel.* = 0;
    sel.* |= RequestedPrivilegeLevel;
    sel.* |= TableIndicator;
    sel.* |= (Index << 3);

    return true;
}

// x86 Segment Descriptor
// 64-bits / 8 bytes
// data1 00:15 - Segment Limit 00:15
// data1 16:31 - Base Address 00:15
// data2 00:07 - Base Address 16:23
// data2 08:11 - Segment Type
// data2 12    - Descriptor Type (0 = system; 1 = code or data)
// data2 13:14 - Descriptor Privilege Level
// data2 15    - Segment Present Bit
// data2 16:19 - Segment Limit 16:19
// data2 20    - Available for our use
// data2 21    - 64-bit code segment
// data2 22    - Default Operation Size (0 = 16-bit segment; 1 = 32-bit segment)
// data2 23    - Granularity (0 = limit is in bytes; 1 = limit is in 4KB units)
// data2 24:31 - Base Address 24:31
pub const SegmentDescriptor = packed struct {
    data1: u32,
    data2: u32,
};

pub const SEG_DESCR_DATA = 0b0000;
pub const SEG_DESCR_CODE = 0b1000;
pub const SEG_DESCR_ACCESSED = 0b0001;
pub const SEG_DESCR_DATA_WRITE = 0b0010;
pub const SEG_DESCR_CODE_READ = 0b0010; 
pub const SEG_DESCR_DATA_EXPAND_DOWN = 0b0100;
pub const SEG_DESCR_CODE_CONFORMING = 0b0100;

pub const SEG_DESCR_SYS_LDT = 2;
pub const SEG_DESCR_SYS_TSS_AVAILABLE = 9;
pub const SEG_DESCR_SYS_TSS_BUSY = 11;
pub const SEG_DESCR_SYS_CALL_GATE = 12;
pub const SEG_DESCR_SYS_INT_GATE = 14;
pub const SEG_DESCR_SYS_TRAP_GATE = 15;

pub fn PrepareSegmentDescriptor(descr: *SegmentDescriptor,
    SegmentLimit: u32,
    BaseAddress: u32,
    SegmentType: u8,
    SystemSegment: bool,
    DescriptorPrivilegeLevel: u8,
    SegmentPresent: bool,
    x64mode: bool,
    OpSize32: bool,
    Granularity4k: bool) bool {

    if (SegmentLimit > (1 << 20)) {
        return false;
    }

    if (SegmentType > 0b1111) {
        return false;
    }

    if (DescriptorPrivilegeLevel > 0x03) {
        return false;
    }

    descr.data1 = 0;
    descr.data2 = 0;

    descr.data1 |= (SegmentLimit & 0x0000FFFF);
    descr.data1 |= ((BaseAddress & 0x0000FFFF) << 16);
    descr.data2 |= ((BaseAddress & 0x00FF0000) >> 16);
    descr.data2 |= (SegmentType << 8);
    
    if (!SystemSegment) {
        descr.data2 |= (1 << 12);
    }

    descr.data2 |= (DescriptorPrivilegeLevel << 13);

    if (SegmentPresent) {
        descr.data2 |= (1 << 15);
    }

    descr.data2 |= ((SegmentLimit & 0x00FF0000) >> 16);

    if (x64mode) {
        descr.data2 |= (1 << 21);
    }

    if (OpSize32) {
        descr.data2 |= (1 << 22);
    }

    if (Granularity4k) {
        descr.data2 |= (1 << 23);
    }

    descr.data2 |= (BaseAddress & 0xFF000000);

    return true;
}

// Pseudo Descriptor
// Note: align mod 4
pub const PseudoDescriptor = packed struct {
    limit: u16,
    base: u64,
};

pub fn PreparePseudoDescriptor(pdescr: *PseudoDescriptor,
    BaseAddress: u64,
    Limit: u16) bool {

    pdescr.base = BaseAddress;
    pdescr.limit = Limit;

    return true;
}

// Page Directory Entry
pub const PageDirectoryEntry = packed struct {
    data: u64,
};

// The Dirty, Global, PAT & ProtectionKey flags are 
// ignored if the instance describes a page table
pub fn PreparePageDirectoryEntry(entry: *PageDirectoryEntry,
    Present: bool,
    ReadWrite: bool,
    Supervisor: bool,
    WriteThrough: bool,
    CacheDisable: bool,
    Accessed: bool,
    Dirty: bool,
    PageTable: bool,
    Global: bool,
    PAT: bool,
    PageAddress: u64,
    ProtectionKey: u8,
    ExecuteDisable: bool) bool {

    if (PageTable and PageAddress > (1 << (PhysicalAddressBits - 12))) {
        return false;
    }

    if (!PageTable and PageAddress > (1 << (PhysicalAddressBits - 21))) {
        return false;
    }

    if (ProtectionKey > (1 << 4)) {
        return false;
    }

    entry.data = 0;

    if (Present) {
        entry.data |= 0x01;
    }

    if (ReadWrite) {
        entry.data |= (1 << 1);
    }

    if (!Supervisor) {
        entry.data |= (1 << 2);
    }

    if (WriteThrough) {
        entry.data |= (1 << 3);
    }

    if (CacheDisable) {
        entry.data |= (1 << 4);
    }

    if (Accessed) {
        entry.data |= (1 << 5);
    }

    if (PageTable) {
        entry.data |= (PageAddress << 12);
    } else {
        entry.data |= (1 << 7);
        entry.data |= (PageAddress << 21);
        entry.data |= (ProtectionKey << 59);

        if (Dirty) {
            entry.data |= (1 << 6);
        }

        if (Global) {
            entry.data |= (1 << 8);
        }

        if (PAT) {
            entry.data |= (1 << 12);
        }
    }

    if (ExecuteDisable) {
        entry.data |= (1 << 63);
    }

    return true;
}

// PDPT Entry
// The Dirty, Global, PAT & ProtectionKey flags are 
// ignored if the instance describes a page directory
pub const PDPTEntry = packed struct {
    data: u64,
};

pub fn PreparePDPTEntry(entry: *PDPTEntry,
    Present: bool,
    ReadWrite: bool,
    Supervisor: bool,
    WriteThrough: bool,
    CacheDisable: bool,
    Accessed: bool,
    Dirty: bool,
    PageDirectory: bool,
    Global: bool,
    PAT: bool,
    PhysicalAddress: u64,
    ProtectionKey: u8,
    ExecuteDisable: bool) bool {

    if (PageDirectory and PhysicalAddress > (1 << (PhysicalAddressBits - 12))) {
        return false;
    }

    if (!PageDirectory and PhysicalAddress > (1 << (PhysicalAddressBits - 30))) {
        return false;
    }

    if (ProtectionKey > (1 << 4)) {
        return false;
    }

    entry.data = 0;

    if (Present) {
        entry.data |= 0x01;
    }

    if (ReadWrite) {
        entry.data |= (1 << 1);
    }

    if (!Supervisor) {
        entry.data |= (1 << 2);
    }

    if (WriteThrough) {
        entry.data |= (1 << 3);
    }

    if (CacheDisable) {
        entry.data |= (1 << 4);
    }

    if (Accessed) {
        entry.data |= (1 << 5);
    }

    if (PageDirectory) {
        entry.data |= (PhysicalAddress << 12);
    } else {
        entry.data |= (1 << 7);
        entry.data |= (PhysicalAddress << 30);
        entry.data |= (ProtectionKey << 59);

        if (Dirty) {
            entry.data |= (1 << 6);
        }

        if (Global) {
            entry.data |= (1 << 8);
        }

        if (PAT) {
            entry.data |= (1 << 12);
        }
    }

    if (ExecuteDisable) {
        entry.data |= (1 << 63);
    }

    return true;
}

// PML4 Entry (4th Level of Paging)
pub const PML4Entry = packed struct {
    data: u64,
};

pub fn PreparePML4Entry(entry: *PML4Entry,
    Present: bool,
    ReadWrite: bool,
    Supervisor: bool,
    WriteThrough: bool,
    CacheDisable: bool,
    Accessed: bool,
    PhysicalAddress: u64,
    ExecuteDisable: bool) bool {

    if (PhysicalAddress > (1 << (PhysicalAddressBits - 12))) {
        return false;
    }

    if (Present) {
        entry.data |= 0x01;
    }

    if (ReadWrite) {
        entry.data |= (1 << 1);
    }

    if (!Supervisor) {
        entry.data |= (1 << 2);
    }

    if (WriteThrough) {
        entry.data |= (1 << 3);
    }

    if (CacheDisable) {
        entry.data |= (1 << 4);
    }

    if (Accessed) {
        entry.data |= (1 << 5);
    }

    entry.data |= (PhysicalAddress << 12);

    if (ExecuteDisable) {
        entry.data |= (1 << 63);
    }

    return true;
}
