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

pub const PAGE_SIZE_4K: u64 = 0x1000;
pub const PAGE_SIZE_2M: u64 = 0x200000;
pub const PAGE_SIZE_1G: u64 = 0x40000000;

const arch = @import("arch/x86_64.zig");
const uefi = @import("kuefi.zig");

pub const uefi_memory_map: ?*uefi.clib.EFI_MEMORY_DESCRIPTOR = null;

const GetMemoryMapPrototype = extern fn (MapSize: ?[*]c_ulonglong, MemoryMap: ?[*]uefi.clib.EFI_MEMORY_DESCRIPTOR, MapKey: ?[*]c_ulonglong, DescriptorSize: ?[*]c_ulonglong, DescriptorVersion: ?[*]c_uint) uefi.clib.EFI_STATUS;
pub const GetMemoryMap: GetMemoryMapPrototype = uefi.clib.gBS.?[0].GetMemoryMap;

// Initialize system memory
pub fn InitMem() void {
    
    // Initialize cpu first
    arch.InitCPU();

    // The first call to GetMemoryMap(...) passes a NULL
    // pointer to obtain the size of the memory
    // map
    var memmap_size: u64 = 0;
    var memmap_key: u64 = 0;
    var memmap_descriptor_size: u64 = 0;
    var memmap_descriptor_version: u64 = 0;


    var efi_result = GetMemoryMap(&memmap_size, 
        null,
        &memmap_key, 
        &memmap_descriptor_size, 
        &memmap_descriptor_version);
}
