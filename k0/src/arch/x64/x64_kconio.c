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

// Uefi Library
#include <Uefi.h>
#include <Library/UefiLib.h>
#include <Library/BaseMemoryLib.h>

#include "../../include/arch/x64/x64_kconio.h"
#include "../../include/kstring.h"

// Pointer to the start of the video buffer
CONST EFI_PHYSICAL_ADDRESS* CONST x64_kconsole = (EFI_PHYSICAL_ADDRESS*)(0xB8000);

// Size of the console video memory 
#define X64_CONSOLE_MEM_SIZE    SIZE_32KB

// Console attributes
UINT8 x64_kconsole_attribute = 0x07;
UINT8 x64_kconsole_currentX = 0;
UINT8 x64_kconsole_currentY = 0;

// Set the background color
BOOLEAN SetBGColor(UINT8 bgcolor) {
    if (bgcolor > 7) {
        return FALSE;
    }

    x64_kconsole_attribute = (x64_kconsole_attribute & 0x8F) | (bgcolor << 4);
    return TRUE;
}

// Set the foreground color
BOOLEAN SetFGColor(UINT8 fgcolor) {
    if (fgcolor > 15) {
        return FALSE;
    }

    x64_kconsole_attribute = (x64_kconsole_attribute & 0xF0) | (fgcolor & 0x0F);
    return TRUE;
}

// Clears the screen :)
VOID ClearScreen() {
    
    UINT32 count;
    EFI_PHYSICAL_ADDRESS *tmp_vid_ptr = x64_kconsole;

    for (count = 0; count < X64_CONSOLE_MEM_SIZE; count++) {
        *tmp_vid_ptr = 0x20;
        tmp_vid_ptr++;
        *tmp_vid_ptr = x64_kconsole_attribute;
        tmp_vid_ptr++;
    }
    x64_kconsole_currentX = x64_kconsole_currentY = 0;
    MoveCursorXY(0, 0);
}

// Move the cursor position to the
// specified x,y value
VOID MoveCursorXY(UINT32 x, UINT32 y) {

    UINT32 crtc_addr = 0x3D4;
    UINT32 offset;

    if (x > CONSOLE_SCREEN_X_MAX)
        return;

    if (y > CONSOLE_SCREEN_Y_MAX)
        return;

    // 80 characters per line
    offset = x + y * CONSOLE_SCREEN_X_SIZE;
    
    // MSB of offset to CRTC reg 14
    x64AsmOutportB(crtc_addr + 0, 14);
    x64AsmOutportB(crtc_addr + 1, offset >> 8);

    /* LSB of offset to CRTC reg 15 */
    x64AsmOutportB(crtc_addr + 0, 15);
    x64AsmOutportB(crtc_addr + 1, offset);
}

// Writes an ASCII string to the console at the specified
// position
VOID WriteStringXY(UINT32 x, UINT32 y, CONST CHAR8 *s) {
    INT32 length, offset, count = 0;
    EFI_PHYSICAL_ADDRESS *video_ptr = NULL;

    if (x > CONSOLE_SCREEN_X_MAX || y > CONSOLE_SCREEN_Y_MAX) {
        return;
    }

    length = kStrlen(s);

    if (length == 0) {
        return;
    }

    offset = (y + CONSOLE_SCREEN_X_SIZE) + x;
    offset *= 2;

    for (count = 0; count < length; ++count) {
        video_ptr = (EFI_PHYSICAL_ADDRESS*)(x64_kconsole + offset + (count * 2));
        *video_ptr = s[count];
        video_ptr++;
        *video_ptr = x64_kconsole_attribute;
    }
}

// Writes an integer as hexadecimal to the console at the
// specified x,y position
VOID WriteHexXY(UINT32 x, UINT32 y, UINT64 val) {
    CHAR8 s[17], *p, c;

    s[16] = 0;
    s[15] = '0';
    p = &s[15];
    if (val) {
        while (val) {
            if ((c = val % 16 + '0') > '9') c += 'A' - '9' - 1;
            *p-- = c;
            val /= 16;
        };
        p++;
    };
    WriteStringXY(x, y, p);
}

// Writes an unsigned integer to the console at the 
// specified x,y position
VOID WriteIntUnsignedXY(UINT32 x, UINT32 y, UINT64 d) {
    char s[22], *p;

    s[21] = 0;
    s[20] = '0';
    p = &s[20];
    if (d) {
        while (d) {
            *p-- = d % 10 + '0';
            d /= 10;
        };
        p++;
    };
    WriteStringXY(x, y, p);
}

// Writes a signed integer to the console at the 
// specified x,y position
VOID WriteIntXY(UINT32 x, UINT32 y, INT64 d) {
    char s[21], *p;

    s[20] = 0;
    s[19] = '0';
    p = &s[19];
    if (d) {
        while (d) {
            *p-- = d % 10 + '0';
            d /= 10;
        };
        if (d < 0) {
            *p-- = '-';
        }
        p++;
    };

    WriteStringXY(x, y, p);
}

// Writes a character to the console at the specified position
VOID WriteCharXY(UINT32 x, UINT32 y, CHAR8 c) {
    int offset;
    EFI_PHYSICAL_ADDRESS *video_ptr;

    if (x > CONSOLE_SCREEN_X_MAX)
        return;
    if (y > CONSOLE_SCREEN_Y_MAX)
        return;

    offset = (y * CONSOLE_SCREEN_X_SIZE) + x;
    offset *= 2;

    video_ptr = (EFI_PHYSICAL_ADDRESS*)(x64_kconsole + offset);

    *video_ptr = c;
    video_ptr++;
    *video_ptr = x64_kconsole_attribute;
}

// Clears a line in the console
VOID ClearLine(UINT32 line) {
    UINT32 i;

    if (line > CONSOLE_SCREEN_Y_SIZE) {
        return;
    }

    for (i = 0; i < CONSOLE_SCREEN_X_SIZE; ++i) {
        WriteStringXY(i, line, " ");
    }
}

// Increments our internal x screen position value
VOID X64InternalIncrementX() {
    if (x64_kconsole_currentX < CONSOLE_SCREEN_X_MAX) {
        x64_kconsole_currentX++;
        MoveCursorXY(x64_kconsole_currentX, x64_kconsole_currentY);
        return;
    }

    if (x64_kconsole_currentY < CONSOLE_SCREEN_Y_MAX) {
        x64_kconsole_currentX = 0;
        x64_kconsole_currentY++;
    }
    else {
        x64_kconsole_currentX = 0;
        x64_kconsole_currentY = CONSOLE_SCREEN_Y_MAX;
        CopyMem(x64_kconsole + (CONSOLE_SCREEN_X_SIZE * 2), 
            x64_kconsole,
            CONSOLE_SCREEN_X_SIZE * (CONSOLE_SCREEN_Y_SIZE - 1) * 2);
        ClearLine(x64_kconsole_currentY);
    }

    MoveCursorXY(x64_kconsole_currentX, x64_kconsole_currentY);
}

// PutChar function
VOID PutChar(CONST CHAR8 c) {
    if (c == '\n') {
        x64_kconsole_currentX = CONSOLE_SCREEN_X_MAX;
    }
    else {
        WriteCharXY(x64_kconsole_currentX, x64_kconsole_currentY, c);
    };
    X64InternalIncrementX();
}

// Puts function
VOID Puts(CONST CHAR8 *s) {
    if (s[0] == '\0') {
        return;
    }

    do {
        PutChar(s[0]);
        s++;

    } while (s[0] != '\0');
}
