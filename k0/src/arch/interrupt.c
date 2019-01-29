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

#include "../include/klib/interrupt.h"

volatile UINT64 isr_fired = 0;
volatile UINT64 page_fault_count = 0;

VOID IsrHandler(unsigned int vector) {
    isr_fired++;

#ifdef __NEBULAE_ARCH_X64
    x64WriteIoApic(X64_APIC_EOI_REG_OFFSET, X64_APIC_END_OF_INTERRUPT);
#endif
}

VOID ExceptionHandler(UINT64 vector, UINT64 error_code) {
    if (vector == 0x0E) {
        page_fault_count++;
        //kernel_panic(L"Page fault exception: %u, error code == 0x%lx\n", vector, error_code);
    }
    else {
        kernel_panic(L"Unhandled exception: %u, error code == 0x%lx\n", vector, error_code);
    }
}

VOID NMIHandler() {
    kernel_panic(L"NMI Detected\n");
}

VOID Exception00() {
    kernel_panic(L"\nException 0x00 -- Divide by Zero\n");
}

VOID Exception01() {
    kernel_panic(L"\nException 0x01 -- Debug\n");
}

VOID Exception03() {
    kernel_panic(L"\nException 0x03 -- Breakpoint\n");
}

VOID Exception04() {
    kernel_panic(L"\nException 0x04 -- Overflow\n");
}

VOID Exception05() {
    kernel_panic(L"\nException 0x05 -- BOUND Range Exceeded\n");
}

VOID Exception06() {
    kernel_panic(L"\nException 0x06 -- Invalid or Undefined Opcode\n");
}

VOID Exception07() {
    kernel_panic(L"\nException 0x07 -- Device not Available (no math coprocessor)\n");
}

VOID Exception08(UINT32 err_code) {
    kernel_panic(L"\nException 0x08 -- Double Fault\n");
}

VOID Exception09() {
    kernel_panic(L"\nException 0x09 -- Coprocessor Segment Overrun\n");
}

VOID Exception0A(UINT32 err_code) {
    kernel_panic(L"\nException 0x0A -- Invalid TSS\n");
}

VOID Exception0B(UINT32 err_code) {
    kernel_panic(L"\nException 0x0B -- Segment Not Present\n");
}

VOID Exception0C(UINT32 err_code) {
    kernel_panic(L"\nException 0x0C -- Stack-Segment Fault\n");
}

VOID Exception0D(UINT32 err_code) {
    kernel_panic(L"\nException 0x0D -- General Protection Fault\n");
}

VOID Exception0E(UINT32 err_code, UINT64 rip, UINT64 cs, UINT64 rflags) {
    UINT8  present, read_write, user_sup, reserved;
    UINT64 fault_address;

    present = (err_code & 0x01);
    read_write = ((err_code >> 1) & 1);
    user_sup = ((err_code >> 2) & 1);
    reserved = ((err_code >> 3) & 1);
    fault_address = x64ReadCR2();

    Print(L"\n\nException 0x0E -- Page Fault [err_code == 0x%08x]\n", err_code);
    Print(L"=====================================================\n");
    if (user_sup == 1)
        Print(L"User mode ");
    else
        Print(L"Supervisor mode ");

    if (read_write == 1)
        Print(L"write ");
    else
        Print(L"read ");

    if (present == 1)
        Print(L"of page protected memory ");
    else
        Print(L"of non-present memory ");

    Print(L"at 0x%08x\nException at CS == 0x%02x | RIP == 0x%lx | RFLAGS == 0x%lx\n", fault_address, cs, rip, rflags);

    if (reserved == 1)
        Print(L"This fault was caused by a reserved bit violation.\n");

    kernel_panic(L"");
}


VOID Exception10() {
    kernel_panic(L"\nException 0x10 -- x87 Floating Point Error\n");
}


VOID Exception11(UINT32 err_code) {
    kernel_panic(L"\nException 0x11 -- Alignment Check\n");
}


VOID Exception12() {
    kernel_panic(L"\nException 0x12 -- Machine Check\n");
}

VOID Exception_13() {
    kernel_panic(L"\nException 0x13 -- SIMD Floating Point Exception\n");
}
