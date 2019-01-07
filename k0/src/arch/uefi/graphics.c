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

#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>

// Kernel Header
#include "../../include/k0.h"

// Graphics Header
#include "../../include/arch/uefi/graphics.h"

// Graphics Options
#define GOP_DESIRED_HREZ            1024
#define GOP_DESIRED_VREZ            768
#define GOP_DESIRED_PIXEL_FORMAT    PixelBlueGreenRedReserved8BitPerColor

VOID InitGraphics() {
    UINTN mode_num;

    gfx_info.handle_count = 0;
    
    EFI_STATUS status = gBS->LocateHandleBuffer(ByProtocol,
        &gEfiGraphicsOutputProtocolGuid,
        NULL,
        &gfx_info.handle_count,
        &gfx_info.handle_buffer);

    if (EFI_ERROR(status)) {
        kernel_panic(L"Error Locating Graphics Output Protocol!\n");
    }

    status = gBS->HandleProtocol(gfx_info.handle_buffer[0],
        &gEfiGraphicsOutputProtocolGuid,
        (VOID **)&gfx_info.gop);

    if (EFI_ERROR(status)) {
        kernel_panic(L"Unable to acquire handle to Graphics Output Protocol!\n");
    }

    for (mode_num = 0;
        (status = gfx_info.gop->QueryMode(gfx_info.gop, mode_num, &gfx_info.size_of_info, &gfx_info.gop_mode_info)) == EFI_SUCCESS;
        mode_num++) {
        if (gfx_info.gop_mode_info->HorizontalResolution == GOP_DESIRED_HREZ &&
            gfx_info.gop_mode_info->VerticalResolution == GOP_DESIRED_VREZ &&
            gfx_info.gop_mode_info->PixelFormat == GOP_DESIRED_PIXEL_FORMAT)
            break;
    }

    if (EFI_ERROR(status)) {
        kernel_panic(L"Unable to locate a suitable display mode in Graphics Output Protocol!\n");
    }

    if (EFI_ERROR(gfx_info.gop->SetMode(gfx_info.gop, mode_num))) {
        kernel_panic(L"Setting the graphics mode of the Graphics Output Protocol failed!\n");
    }

    boot_display.width = GOP_DESIRED_HREZ;
    boot_display.height = GOP_DESIRED_VREZ;
}

VOID drawTriangle(EFI_PHYSICAL_ADDRESS lfb_base_addr, UINTN center_x, UINTN center_y, UINTN width, UINT32 color) {
    UINT32* at = (UINT32*)lfb_base_addr;
    UINTN row, col;

    at += (GOP_DESIRED_HREZ * (center_y - width / 2) + center_x - width / 2);

    for (row = 0; row < width / 2; row++) {
        for (col = 0; col < width - row * 2; col++)
            *at++ = color;
        at += (GOP_DESIRED_HREZ - col);
        for (col = 0; col < width - row * 2; col++)
            *at++ = color;
        at += (GOP_DESIRED_HREZ - col + 1);
    }
}
