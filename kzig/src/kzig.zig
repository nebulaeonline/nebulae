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

// Zig Compiler Builtins
const builtin = @import("builtin");

// Additional imports
const klib = @import("klib.zig");
const kmem = @import("kmem.zig");
const uefi = @import("kuefi.zig");

// True entrypoint to Zig code
export fn kzig_main() noreturn {

    klib.kzig_main_called = true;

    // Print a welcome message
    const m1 = c"Welcome to nebulae on zig!\n";
    var efi_result = uefi.clib.AsciiPrint(@ptrCast(?[*]const u8, m1));
    
    // Initialize CSPRNG (isaac64)
    uefi.clib.InitIsaac64CSPRNG(1);
    const m2 = c"CSPRNG initialized\n";
    efi_result = uefi.clib.AsciiPrint(@ptrCast(?[*]const u8, m2));

    // Initialize memory subsystem
    kmem.InitMem();
    const m3 = c"Memory subsystem initialized\n";
    efi_result = uefi.clib.AsciiPrint(@ptrCast(?[*]const u8, m3));

    while (true) {}
}
