; Copyright (c) 2005-2019 Nebulae Foundation. All rights reserved.
; Copyright (c) 2006, Intel Corporation. All rights reserved.
;
; Redistribution and use in source and binary forms, with or without 
; modification, are permitted provided that the following conditions are met:
; 
; 1. Redistributions of source code must retain the above copyright notice, 
; this list of conditions and the following disclaimer.
; 
; 2. Redistributions in binary form must reproduce the above copyright notice, 
; this list of conditions and the following disclaimer in the documentation 
; and/or other materials provided with the distribution.
; 
; THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" 
; AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE 
; IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE 
; ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE 
; LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR 
; CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF 
; SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS 
; INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN 
; CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) 
; ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE 
; POSSIBILITY OF SUCH DAMAGE.
;==================================

global x64OutportB, x64OutportW
global x64InportB, x64InportW
global x64EnableInterrupts, x64DisableInterrupts
global x64ReadTsc, x64LoadStackSegmentAndJump
global x64WriteCR3, x64ReadGdtr, x64WriteGdtr
global x64ReadIdtr, x64WriteIdtr, x64ReadCR2
global x64ReadCS, x64ReadDS, x64ReadSS
global x64ReadES, x64ReadFS, x64ReadGS
global x64LoadTR, x64InvalidatePage, x64AtomicOr
global x64AtomicAnd, x64AtomicXor, x64AtomicAdd
global x64AtomicSub, x64AtomicInc, x64AtomicDec
global x64AtomicDecAndTestZero, 
global x64SpinlockAcquire, x64SpinlockRelease

bits 64
%define retfq o64 retf

segment .text

;==================================
; VOID x64OutportB(
;   UINT16 Port, 
;   UINT8  Value);
;
x64OutportB:
    mov rax, rcx
    mov rbx, rdx

    mov dx, ax
    mov al, bl

    out dx, al

    ret

;==================================
; VOID x64OutportW(
;   UINT16 Port, 
;   UINT16 Value);
;
x64OutportW:
    mov rax, rcx
    mov rbx, rdx

    mov dx, ax
    mov ax, bx

    out dx, ax

    ret

;==================================
; UINT8 x64InportB(
;   UINT16 Port);
;
x64InportB:
    mov rax, rcx
    mov dx, ax

    in al, dx

    ret

;==================================
; UINT16 x64InportW(
;   UINT16 Port);
;
x64InportW:
    mov rax, rcx
    mov dx, ax

    in eax, dx

    ret

;==================================
; VOID x64EnableInterrupts();
;
x64EnableInterrupts:
    cli
    ret

;==================================
; VOID x64DisableInterrupts();
;
x64DisableInterrupts:
    sti
    ret

;==================================
; UINT64
; EFIAPI
; x64ReadTsc ();
;
x64ReadTsc:
    rdtsc
    shl     rdx, 20h
    or      rax, rdx
    ret

;==================================
; UINTN
; EFIAPI
; x64WriteCR3 (UINTN  new_cr3);
;
x64WriteCR3:
    mov     cr3, rcx
    mov     rax, rcx
    ret

;==================================
; VOID
; EFIAPI
; x64ReadGdtr (
;   OUT x64_seg_sel  *gdtr);
;
x64ReadGdtr:
    sgdt    [rcx]
    ret

;==================================
; VOID
; EFIAPI
; x64WriteGdtr (
;   IN CONST x64_seg_sel *gdtr);
;
x64WriteGdtr:
    lgdt    [rcx]
    ret

;==================================
; VOID
; EFIAPI
; x64ReadIdtr (
;   OUT x64_seg_sel  *idtr);
;
x64ReadIdtr:
    sidt    [rcx]
    ret

;==================================
; VOID
; EFIAPI
; x64WriteIdtr (
;   IN CONST x64_seg_sel *idtr);
;
x64WriteIdtr:
    lidt    [rcx]
    ret

;==================================
; UINT64
; EFIAPI
; x64ReadCR2 (VOID);
;
x64ReadCR2:
    mov     rax, cr2
    ret

;==================================
; UINT16
; EFIAPI
; x64ReadCS ();
;
x64ReadCS:
    xor     rax, rax
    mov     ax, cs
    ret

;==================================
; UINT16
; EFIAPI
; x64ReadDS ();
;
x64ReadDS:
    xor     rax, rax
    mov     ax, ds
    ret

;==================================
; UINT16
; EFIAPI
; x64ReadSS ();
;
x64ReadSS:
    xor     rax, rax
    mov     ax, ss
    ret

;==================================
; UINT16
; EFIAPI
; x64ReadES ();
;
x64ReadES:
    xor     rax, rax
    mov     ax, es
    ret

;==================================
; UINT16
; EFIAPI
; x64ReadFS ();
;
x64ReadFS:
    xor     rax, rax
    mov     ax, fs
    ret

;==================================
; UINT16
; EFIAPI
; x64ReadGS ();
;
x64ReadGS:
    xor     rax, rax
    mov     ax, es
    ret

;==================================
; VOID
; EFIAPI
; x64LoadTR (
;   UINT16 tr_gdt_descr_index);
;
x64LoadTR:
    ltr     cx
    ret

;==================================
; VOID 
; EFIAPI 
; x64InvalidatePage(
;   EFI_PHYSICAL_ADDRESS *addr);
;
x64InvalidatePage:
    invlpg  [rcx]
    ret

;==================================
; VOID
; EFIAPI
; x64AtomicOr(
;   UINT64 *value, 
;   UINT64 mask);
;
x64AtomicOr:
    lock or qword [rcx], rdx
    ret

;==================================
; VOID
; EFIAPI
; x64AtomicAnd(
;   UINT64 *value, 
;   UINT64 mask);
;
x64AtomicAnd:
    lock and qword [rcx], rdx
    ret

;==================================
; VOID 
; EFIAPI
; x64AtomicXor(
;   UINT64 *value, 
;   UINT64 mask);
;
x64AtomicXor:
    lock xor qword [rcx], rdx
    ret
	
;==================================
; VOID
; EFIAPI
; x64AtomicAdd(
;   UINT64 *value, 
;   UINT64 addend);
;
x64AtomicAdd:
	lock add qword [rcx], rdx
    ret

;==================================
; VOID
; EFIAPI
; x64AtomicSub(
;   UINT64 *value, 
;   UINT64 subtrahend);
;
x64AtomicSub:
    lock sub qword [rcx], rdx
    ret

;==================================
; VOID
; EFIAPI
; x64AtomicInc(UINT64 *value);
;
x64AtomicInc:
    lock inc qword [rcx]
    ret

;==================================
; VOID
; EFIAPI
; x64AtomicDec(UINT64 *value);
;
x64AtomicDec:
    lock dec qword [rcx]
    ret

;==================================
; UINT8 
; EFIAPI
; x64AtomicDecAndTestZero(
;   UINT64 *value);
;
x64AtomicDecAndTestZero:
    lock dec qword [rcx]
    xor rax, rax
	setz al
	ret

;==================================
; VOID
; EFIAPI
; x64SpinlockAcquire(
;   VOID *spinlock);
;
x64SpinlockAcquire:
	cmp qword [rcx], 0
	je x64_s_a_get_spinlock
	pause
	jmp x64SpinlockAcquire
	
x64_s_a_get_spinlock:
	mov qword rbx, 1
	xchg [rcx], rbx
	cmp qword rbx, 0
	jne x64SpinlockAcquire
	ret

;==================================	
; VOID
; EFIAPI
; x64SpinlockRelease(
;   VOID *spinlock);
;
x64SpinlockRelease:
	mov qword rbx, 0
	xchg [rcx], rbx
	ret
	