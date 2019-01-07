// Copyright (c) 2018-2019 Nebulae Foundation. All rights reserved.
// Contains code Copyright (c) 2015  Finnbarr P. Murphy.   All rights reserved.
// Read more : https://blog.fpmurphy.com/2015/01/list-acpi-tables-from-uefi-shell.html
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

//
// Basic UEFI Libraries
//
#include <Library/UefiLib.h>
#include <Library/PrintLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>

#ifdef __NEBULAE_ARCH_X64
#include "../include/arch/x64/kmem.h"
#endif

UINTN kStrnCmpA(CHAR8 *s1, CHAR8 *s2, UINTN len) {
    
    while (*s1 && len) {
        if (*s1 != *s2) {
            break;
        }
        s1 += 1;
        s2 += 1;
        len -= 1;
    }

    return len ? *s1 - *s2 : 0;
}

// Naive and horrible...
UINTN kStrlen(CONST CHAR8* s) {

    const char *sc;

    for (sc = s; *sc != '\0'; ++sc)
        /* do nothing */;
    return (sc - s);
}

VOID kAscii2UnicodeStr(CONST CHAR8 *String, CHAR16 *UniString, UINTN length) {
    
    UINTN len = length;

    while (*String != '\0' && len > 0) {
        *(UniString++) = (CHAR16) *(String++);
        len--;
    }
    *UniString = '\0';
}

VOID kGuid2String(CHAR16 *buffer, UINTN buffsiz, EFI_GUID *guid) {
    
    UnicodeSPrint(buffer, buffsiz, L"%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x",
        guid->Data1,
        guid->Data2,
        guid->Data3,
        guid->Data4[0],
        guid->Data4[1],
        guid->Data4[2],
        guid->Data4[3],
        guid->Data4[4],
        guid->Data4[5],
        guid->Data4[6],
        guid->Data4[7]);
}
