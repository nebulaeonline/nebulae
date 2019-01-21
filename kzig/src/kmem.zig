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

pub const PAGE_SIZE_4K: u64 = 0x1000;
pub const PAGE_SIZE_2M: u64 = 0x200000;
pub const PAGE_SIZE_1G: u64 = 0x40000000;

const arch = @import("arch/x86_64.zig");
const uefi = @import("kuefi.zig");

// Pointer to the memory map
pub const uefi_memory_map: ?*uefi.clib.EFI_MEMORY_DESCRIPTOR = null;

// GetMemoryMap function prototype
const GetMemoryMapPrototype = extern fn (
    MapSize: ?[*]c_ulonglong, 
    MemoryMap: ?[*]uefi.clib.EFI_MEMORY_DESCRIPTOR, 
    MapKey: ?[*]c_ulonglong, 
    DescriptorSize: ?[*]c_ulonglong, 
    DescriptorVersion: ?[*]c_uint
) uefi.clib.EFI_STATUS;

// AllocatePages function prototype
const AllocatePagesPrototype = extern fn (
    AllocateType: uefi.clib.EFI_ALLOCATE_TYPE,
    MemoryType: uefi.clib.EFI_MEMORY_TYPE,
    PageCount: c_ulonglong,
    PhysicalAddress: ?[*]uefi.clib.EFI_PHYSICAL_ADDRESS
) uefi.clib.EFI_STATUS;

// Function pointers
pub var GetMemoryMap: GetMemoryMapPrototype = undefined;
pub var AllocatePages: AllocatePagesPrototype = undefined;

// Initialize system memory
pub fn InitMem() VOID {
    
    // Initialize cpu first
    arch.InitCPU();

    // Assign memory-related function pointers
    GetMemoryMap = uefi.clib.gBS.?[0].GetMemoryMap.?;
    AllocatePages = uefi.clib.gBS.?[0].AllocatePages.?;

    // Local memmap helper variables
    var memmap_size: c_ulonglong = 0;
    var memmap_key: c_ulonglong = 0;
    var memmap_descriptor_size: c_ulonglong = 0;
    var memmap_descriptor_version: c_uint = 0;

    // The first call to GetMemoryMap(...) passes a NULL
    // pointer to obtain the size of the memory map
    var efi_result = GetMemoryMap(@ptrCast([*]c_ulonglong, &memmap_size),
        null,
        @ptrCast([*]c_ulonglong, &memmap_key),
        @ptrCast([*]c_ulonglong, &memmap_descriptor_size),
        @ptrCast([*]c_uint, &memmap_descriptor_version));    // sanity check
    const m1 = c"Memory map size: %lu bytes\n";
    efi_result = uefi.clib.AsciiPrint(@ptrCast(?[*]const u8, m1), memmap_size);
}
