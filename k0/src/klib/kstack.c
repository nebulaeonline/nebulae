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
#include <Library/BaseMemoryLib.h>
#include "../include/klib/kstack.h"

// Initialize a stack structure
// Valid direction for stack (dir) is 1 or -1
nebStatus kInitStackStructure(kstack *stack, EFI_PHYSICAL_ADDRESS* base_addr, UINTN size_in_bytes, INT32 dir) {
    
    // Verify no nulls
    if (ISNULL(stack) || ISNULL(base_addr)) {
        return NEBERROR_NULL_PTR_UNEXPECTED;
    }

    // Only accept 1 or -1
    if (dir != -1 && dir != 1) {
        return NEBERROR_BAD_STACK_DIRECTION;
    }

    // Stack size should be a multiple of KSTACK_UNIT_SIZE
    if ((size_in_bytes % KSTACK_UNIT_SIZE) != 0) {
        return NEBERROR_BAD_STACK_SIZE;
    }
    
    // Zero the memory
    if (ZeroMem(base_addr, size_in_bytes) != base_addr) {
        return NEBERROR_MEM_ZERO_ERR;
    }

    if (dir > 0) {
        stack->base = stack->top = base_addr;
    }
    else {
        stack->base = stack->top = (EFI_PHYSICAL_ADDRESS*)((UINT64)base_addr + size_in_bytes);
    }

    stack->size = size_in_bytes;
    stack->dir = dir;

    return NEB_OK;    
}

// Push a value onto the stack
// Returns the address of the object's location in the structure
EFI_PHYSICAL_ADDRESS* kStackPush(kstack *stack, UINT64 value) {
    
    // can't do anything if the passed address is NULL
    if (ISNULL(stack)) {
        return NULL;
    }

    // see if the stack is already full
    // remember top is always 1 step ahead
    if (stack->dir == KSTACK_DIRECTION_GROW_UP && (UINT64)stack->top >= ((UINT64)stack->base + ((UINT64)stack->dir * stack->size))) {
        Print(L"Grow up stack top (@ 0x%lx) too big. base == 0x%lx / size == %lu\n", stack->top, stack->base, stack->size);
        return NULL;
    }
    else if (stack->dir == KSTACK_DIRECTION_GROW_DOWN && (UINT64)stack->top <= ((UINT64)stack->base + ((UINT64)stack->dir * stack->size))) {
        Print(L"Grow down stack bottom (@ 0x%lx) too small. base == 0x%lx / eqn == %ld\n", stack->top, stack->base, (EFI_PHYSICAL_ADDRESS*)((UINT64)stack->base + (stack->dir * stack->size)));
        return NULL;
    }

    // Put the value on the stack
    *(stack->top) = value;
    
    // Increment the stack pointer the proper amount and direction
    stack->top += (stack->dir * KSTACK_UNIT_SIZE);

    return (EFI_PHYSICAL_ADDRESS*)(stack->top - (stack->dir * KSTACK_UNIT_SIZE));
}

// Pop a value off the stack
UINT64 kStackPop(kstack *stack) {

    // no NULLs
    if (ISNULL(stack)) {
        return 0;
    }

    // see if there's anything on the stack
    if (stack->top == stack->base) {
        return 0;   // nothing on the stack
    }

    // remember top is always 1 step ahead
    UINT64 ret = *(stack->top - (stack->dir * KSTACK_UNIT_SIZE));

    // Move top "down"
    stack->top -= (stack->dir * KSTACK_UNIT_SIZE);

    return ret;
}

// Returns the value on top of the specified stack,
// but does not actually adjust any internal pointers
UINT64 kStackPeek(kstack *stack) {

    // no NULLs
    if (ISNULL(stack)) {
        return 0;
    }

    // see if there's anything on the stack
    if (stack->top == stack->base) {
        return 0;   // nothing on the stack
    }

    // remember top is always 1 step ahead
    // just return the value, pointers stay the same
    return *(stack->top - (stack->dir * KSTACK_UNIT_SIZE));
}

// Returns the count of entries in the stack
UINT64 kGetStackCount(kstack *stack) {
    
    if (stack->top == stack->base) {
        return 0;
    }

    switch (stack->dir) {
    case KSTACK_DIRECTION_GROW_UP:
        return (stack->top - stack->base) / KSTACK_UNIT_SIZE;
    case KSTACK_DIRECTION_GROW_DOWN:
        return (stack->base - stack->top) / KSTACK_UNIT_SIZE;
    default:
        return 0;
    }
}
