// Copyright (c) 2018 Nebulae Foundation. All rights reserved.
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

pub var PhysicalAddressBits: u8 = 0;
pub var LinearAddressBits: u8 = 0;

// Function to query the processor to determine the physical and linear
// address bits supported
pub fn InitAddressBits() void {
    var data: u32 = 0;
    var retval: u32 = 0;
    
    // This cpuid function returns the maximum supported EAX value supported
    // by this cpu's cpuid function (hmmm...)
    retval = baselib.AsmCpuid(0x80000000, @ptrCast(?[*]c_uint, &data), null, null, null);
    if (data < 0x80000008) {
        @panic("Unable to determine CPU bit support (hardware & logical)");
    }
    
    retval = baselib.AsmCpuid(0x80000008, @ptrCast(?[*]c_uint, &data), null, null, null);

    PhysicalAddressBits = @truncate(u8, (data & 0x000000FF));
    LinearAddressBits = @truncate(u8, ((data & 0x0000FF00) >> 8));
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
