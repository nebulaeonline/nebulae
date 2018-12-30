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

// UEFI Includes
pub const uefi = @cImport({ 
    // Basic UEFI Includes
    @cInclude("Uefi.h");
    @cInclude("Library/UefiLib.h");
    @cInclude("Library/DebugLib.h"); 
    @cInclude("Library/MemoryAllocationLib.h");
    @cInclude("Library/BaseMemoryLib.h");

    // Boot and Runtime Services
    @cInclude("Library/UefiBootServicesTableLib.h");
    @cInclude("Library/UefiRuntimeServicesTableLib.h");

    // Shell Library
    @cInclude("Library/ShellLib.h");

    // CSPRNG
    // UINT64 GetCSPRNG64(UINT64 min, UINT64 max)
    @cInclude("isaac64.h");
});

// Support any version of UEFI
export const _gUefiDriverRevision: u32 = 0;

// Lazy binding...
pub var kzig_main_called: bool = false;

// Zig entrypoint
extern fn kzig_main() noreturn;

// Locations of important uefi structures
var efi_image_handle: uefi.EFI_HANDLE = undefined;
var efi_system_table: [*]uefi.EFI_SYSTEM_TABLE = undefined;

// Uefi entrypoint
export fn UefiMain(image_handle: uefi.EFI_HANDLE, system_table: [*]uefi.EFI_SYSTEM_TABLE) uefi.EFI_STATUS {

    // Store our uefi image handle and system table location
    efi_image_handle = image_handle;
    efi_system_table = system_table;

    // Jump to our true Zig kernel entrypoint
    kzig_main();
}

// We can't rely on the built-in memcpy linking properly
// if it gets brought in. TODO figure out why (it's also a builtin 
// on msvc, so that prevents us from implementing memcpy in C).
export fn memcpy(noalias dest: ?[*]u8, noalias src: ?[*]const u8, n: usize) ?[*]u8 {
    var efi_result = uefi.CopyMem(@ptrCast(?*c_void, dest), @ptrCast(*const c_void, src), n);
    return dest;
}

// Panic implementation that relies on UEFI output
pub fn panic(msg: []const u8, error_return_trace: ?*builtin.StackTrace) noreturn {
    @setCold(true);
    var efi_result = uefi.AsciiPrint(@ptrCast(?[*]const u8, &msg));
    while (true) {}
}
