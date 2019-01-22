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

#ifndef __K0_GRAPHICS_H
#define __K0_GRAPHICS_H

typedef struct s_uefi_gfx_info {
    EFI_HANDLE* handle_buffer;
    UINTN handle_count;
    EFI_GRAPHICS_OUTPUT_PROTOCOL* gop;
    EFI_GRAPHICS_OUTPUT_MODE_INFORMATION* gop_mode_info;
    UINTN size_of_info;
} uefi_gfx_info;

uefi_gfx_info gfx_info;

typedef struct s_uefi_display {
    UINT32 width;
    UINT32 height;
} uefi_display;

uefi_display boot_display;

VOID InitGraphics();

VOID drawTriangle(EFI_PHYSICAL_ADDRESS lfb_base_addr, 
    UINTN center_x, 
    UINTN center_y, 
    UINTN width, 
    UINT32 color);

#endif // __K0_GRAPHICS_H
