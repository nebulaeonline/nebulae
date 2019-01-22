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

#ifndef __K0_KSTACK_H
#define __K0_KSTACK_H

#include "../k0.h"

// Stack grows upward from base
#define KSTACK_DIRECTION_GROW_UP    1

// Stack grows downward from (base + size)
#define KSTACK_DIRECTION_GROW_DOWN  -1

// We store only 8-byte UINT64s
#define KSTACK_UNIT_SIZE            8ULL

// kstack structure
typedef struct s_kstack {
    UINT64 *base;
    UINT32 size;
    UINT64 *top;
    INT32  dir;
} kstack;

// Initializes a stack structure with appropriate values given the parameters
// size_in_bytes must be a multiple of KSTACK_UNIT_SIZE
nebStatus kInitStackStructure(kstack *stack, 
                              EFI_PHYSICAL_ADDRESS* base_addr, 
                              UINTN size_in_bytes, INT32 dir);

// Push a value onto the specified stack
EFI_PHYSICAL_ADDRESS* kStackPush(kstack *stack, UINT64 value);

// Pop a value off the specified stack
UINT64 kStackPop(kstack *stack);

// Peek at what the next value to be popped will be
UINT64 kStackPeek(kstack *stack);

// Count the number of items in the stack
UINT64 kGetStackCount(kstack *stack);

// Finds a value in the stack, and swaps it with the next value to be popped.
// returns NEBERROR_STACK_ELEMENT_NOT_FOUND if the value is not located within
// the stack.  The size factor indicates the range applied to make a match --
// i.e. SIZE_4KB for 4KB pages, or SIZE_2MB for 2MB pages; this value is
// added to each address in the stack to see if value falls in that range.
// Exact matches would have a size factor of 0.
nebStatus kStackSwapValue(kstack *stack, UINT64 value, UINTN size_factor);

// Finds an item in the stack
nebStatus kStackFindValue(kstack *stack, UINT64 value, UINTN size_factor);

#endif // __K0_KSTACK_H
