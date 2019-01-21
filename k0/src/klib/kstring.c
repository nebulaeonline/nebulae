// Copyright (c) 2005-2019 Nebulae Foundation. All rights reserved.
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

#include "../include/k0.h"

// Poor man's strncmp
INT64 kStrnCmpA(CHAR8 *s1, CHAR8 *s2, UINTN len) {
    
    if (ISNULL(s1) || ISNULL(s2)) {
        return NEBERROR_NULL_PTR_UNEXPECTED;
    }

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

// use sparingly...
UINT64 kStrlen(CHAR8 *s) {

    CONST CHAR8 *sc;

    if (ISNULL(s)) {
        return 0;
    }

    for (sc = s; *sc != '\0'; ++sc)
        /* do nothing */;
    return (sc - s);
}

// Converts an Ascii string to a unicode string
INT64 kAscii2UnicodeStr(CHAR8 *String, CHAR16 *UniString, UINTN length) {
    
    UINTN len = length;
    
    if (ISNULL(String) || ISNULL(UniString)) {
        return NEBERROR_NULL_PTR_UNEXPECTED;
    }

    while (*String != '\0' && len > 0) {
        *(UniString++) = (CHAR16) *(String++);
        len--;
    }
    *UniString = '\0';

    return NEB_OK;
}

// Converts a guid to a wide character string
INT64 kGuid2String(CHAR16 *buffer, UINTN buffsiz, EFI_GUID *guid) {
    
    if (ISNULL(buffer) || ISNULL(guid)) {
        return NEBERROR_NULL_PTR_UNEXPECTED;
    }

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

    return NEB_OK;
}

// Helper function for json string matching without libc
INT32 jsoneq(CONST CHAR8 *json, jsmntok_t *tok, CONST CHAR8 *s) {
    if (ISNULL(json) || ISNULL(tok) || ISNULL(s)) {
        return NEBERROR_NULL_PTR_UNEXPECTED;
    }

    if (tok->type == JSMN_STRING && (int)kStrlen(s) == tok->end - tok->start &&
        kStrnCmpA(json + tok->start, s, tok->end - tok->start) == 0) {
        return 0;
    }
    return -1;
}
