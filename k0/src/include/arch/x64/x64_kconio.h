// Copyright (c) 2006-2019 Nebulae Foundation. All rights reserved.
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

#ifndef __K0_X64_KCONIO_H
#define __K0_X64_KCONIO_H

#include "x64.h"

// Constant declarations
#define CONSOLE_COLOR_BLACK     0x00
#define CONSOLE_COLOR_BLUE      0x01
#define CONSOLE_COLOR_GREEN     0x02
#define CONSOLE_COLOR_CYAN      0x03
#define CONSOLE_COLOR_RED       0x04
#define CONSOLE_COLOR_MAGENTA   0x05
#define CONSOLE_COLOR_BROWN     0x06
#define CONSOLE_COLOR_WHITE     0x07
#define CONSOLE_COLOR_GRAY      0x08
#define CONSOLE_COLOR_BLUEX     0x09
#define CONSOLE_COLOR_GREENX    0x0A
#define CONSOLE_COLOR_CYANX     0x0B
#define CONSOLE_COLOR_PINK      0x0C
#define CONSOLE_COLOR_MAGENTAX  0x0D
#define CONSOLE_COLOR_YELLOW    0x0E
#define CONSOLE_COLOR_WHITEX    0x0F

#define CONSOLE_SCREEN_X_SIZE	80
#define CONSOLE_SCREEN_Y_SIZE	25
#define CONSOLE_SCREEN_X_MAX	79
#define CONSOLE_SCREEN_Y_MAX	24

// Function prototypes
VOID ClearScreen(VOID);
BOOLEAN SetBGColor(UINT8 bgcolor);
BOOLEAN SetFGColor(UINT8 fgcolor);
VOID MoveCursorXY(UINT32 x, UINT32 y);
VOID WriteStringXY(UINT32 x, UINT32 y, CONST CHAR8 *s);
VOID WriteNumberHex8XY(UINT32 x, UINT32 y, UINT8 val);
VOID WriteNumberHex16XY(UINT32 x, UINT32 y, UINT16 val);
VOID WriteNumberHex32XY(UINT32 x, UINT32 y, UINT32 val);
VOID WriteNumberHex64XY(UINT32 x, UINT32 y, UINT64 val);
VOID WriteNumberDecU8XY(UINT32 x, UINT32 y, UINT8 d);
VOID WriteNumberDecU16XY(UINT32 x, UINT32 y, UINT16 d);
VOID WriteNumberDecU32XY(UINT32 x, UINT32 y, UINT32 d);
VOID WriteNumberDecU64XY(UINT32 x, UINT32 y, UINT64 d);
VOID WriteChar(UINT32 x, UINT32 y, CHAR8 c);
VOID ClearLine(UINT32 line);
VOID PutChar(CONST CHAR8 c);
VOID Puts(CONST CHAR8 *s);

#endif // __K0_X64_KCONIO_H
