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

VOID Exception_0x00();
VOID Exception_0x01();
VOID Exception_0x03();
VOID Exception_0x04();
VOID Exception_0x05();
VOID Exception_0x06();
VOID Exception_0x07();
VOID Exception_0x08();
VOID Exception_0x09();
VOID Exception_0x0A();
VOID Exception_0x0B();
VOID Exception_0x0C();
VOID Exception_0x0D();
VOID Exception_0x0E();
VOID Exception_0x10();
VOID Exception_0x11();
VOID Exception_0x12();
VOID Exception_0x13();
VOID Exception_0x14();

volatile UINT64 isr_fired = 0;
volatile UINT64 page_fault_count = 0;

VOID IsrHandler(unsigned int vector) {
    isr_fired++;

#ifdef __NEBULAE_ARCH_X64
    x64WriteIoApic(X64_APIC_EOI_REG_OFFSET, X64_APIC_END_OF_INTERRUPT);
#endif
}

VOID ExceptionHandler(UINT64 vector, UINT64 error_code) {
    switch (vector) {
    case 0x00:
        Exception_0x00();
        return;
    case 0x01:
        Exception_0x01();
        return;
    case 0x03:
        Exception_0x03();
        return;
    case 0x04:
        Exception_0x04();
        return;
    case 0x05:
        Exception_0x05();
        return;
    case 0x06:
        Exception_0x06();
        return;
    case 0x07:
        Exception_0x07();
        return;
    case 0x08:
        Exception_0x08(error_code);
        return;
    case 0x09:
        Exception_0x09();
        return;
    case 0x0A:
        Exception_0x0A(error_code);
        return;
    case 0x0B:
        Exception_0x0B(error_code);
        return;
    case 0x0C:
        Exception_0x0C(error_code);
        return;
    case 0x0D:
        Exception_0x0D(error_code);
        return;
    case 0x0E:
        Exception_0x0E(error_code);
        return;
    case 0x10:
        Exception_0x10();
        return;
    case 0x11:
        Exception_0x11(error_code);
        return;
    case 0x12:
        Exception_0x12();
        return;
    case 0x13:
        Exception_0x13();
        return;
    case 0x14:
        Exception_0x14();
        return;
    }
}

VOID NMIHandler() {
    kernel_panic(L"NMI Detected\n");
}

VOID Exception_0x00() {
    kernel_panic(L"Exception 0x00 -- Divide by Zero\n");
}

VOID Exception_0x01() {
    kernel_panic(L"Exception 0x01 -- Debug\n");
}

VOID Exception_0x03() {
    kernel_panic(L"Exception 0x03 -- Breakpoint\n");
}

VOID Exception_0x04() {
    kernel_panic(L"Exception 0x04 -- Overflow\n");
}

VOID Exception_0x05() {
    kernel_panic(L"Exception 0x05 -- BOUND Range Exceeded\n");
}

VOID Exception_0x06() {
    kernel_panic(L"Exception 0x06 -- Invalid or Undefined Opcode\n");
}

VOID Exception_0x07() {
    kernel_panic(L"Exception 0x07 -- Device not Available (no math coprocessor)\n");
}

VOID Exception_0x08(UINT32 err_code) {
    kernel_panic(L"Exception 0x08 -- Double Fault; Error Code == %ld\n", err_code);
}

VOID Exception_0x09() {
    kernel_panic(L"\nException 0x09 -- Coprocessor Segment Overrun\n");
}

VOID Exception_0x0A(UINT32 err_code) {
    kernel_panic(L"\nException 0x0A -- Invalid TSS\n");
}

VOID Exception_0x0B(UINT32 err_code) {
    kernel_panic(L"\nException 0x0B -- Segment Not Present\n");
}

VOID Exception_0x0C(UINT32 err_code) {
    kernel_panic(L"\nException 0x0C -- Stack-Segment Fault\n");
}

VOID Exception_0x0D(UINT32 err_code) {
    kernel_panic(L"\nException 0x0D -- General Protection Fault\n");
}

VOID Exception_0x0E(UINT32 err_code) {
    kernel_panic(L"Exception 0x0E -- Page Fault [err_code == 0x%lx] @ 0x%lx\n", err_code, x64ReadCR2());
}

VOID Exception_0x10() {
    kernel_panic(L"\nException 0x10 -- x87 Floating Point Error\n");
}

VOID Exception_0x11(UINT32 err_code) {
    kernel_panic(L"\nException 0x11 -- Alignment Check\n");
}

VOID Exception_0x12() {
    kernel_panic(L"\nException 0x12 -- Machine Check\n");
}

VOID Exception_0x13() {
    kernel_panic(L"\nException 0x13 -- SIMD Floating Point Exception\n");
}

VOID Exception_0x14() {
    kernel_panic(L"\nException 0x14 -- VMX Exception\n");
}
