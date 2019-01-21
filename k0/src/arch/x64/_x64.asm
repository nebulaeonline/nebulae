; Copyright (c) 2003-2019 Nebulae Foundation. All rights reserved.
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

global x64AsmOutportB, x64AsmOutportW
global x64AsmInportB, x64AsmInportW
global x64EnableInterrupts, x64DisableInterrupts
global x64ReadTsc, x64LoadStackSegmentAndJump
global x64WriteCR3, x64ReadGdtr, x64WriteGdtr
global x64ReadCS, x64ReadDS, x64ReadSS
global x64ReadES, x64ReadFS, x64ReadGS

bits 64
segment .text

;==================================
; VOID x64AsmOutportB(
;   UINT16 Port, 
;   UINT8  Value)
;
x64AsmOutportB:
    mov rax,rcx
    mov rbx,rdx

    mov dx,ax
    mov al,bl

    out dx,al

    xor eax,eax

    ret

;==================================
; VOID x64AsmOutportW(
;   UINT16 Port, 
;   UINT16 Value)
;
x64AsmOutportW:
    mov rax,rcx
    mov rbx,rdx

    mov dx,ax
    mov ax,bx

    out dx,ax

    xor eax,eax

    ret

;==================================
; UINT8 x64AsmInportB(
;   UINT16 Port)
;
x64AsmInportB:
    mov rax,rcx
    mov dx,ax

    in al,dx

    ret

;==================================
; UINT16 x64AsmInportW(
;   UINT16 Port)
;
x64AsmInportW:
    mov rax,rcx
    mov dx,ax

    in eax,dx

    ret

;==================================
; VOID x64EnableInterrupts()
;
x64EnableInterrupts:
    cli
    xor eax,eax
    ret

;==================================
; VOID x64DisableInterrupts()
;
x64DisableInterrupts:
    sti
    xor eax,eax
    ret

;==================================
; UINT64
; EFIAPI
; x64ReadTsc (VOID)
;
x64ReadTsc:
    rdtsc
    shl     rdx, 20h
    or      rax, rdx
    ret

;==================================
; NORETURN
; x64_farptr*
; EFIAPI
; x64LoadStackSegmentAndJump (
;   x64_farptr*,
;   VOID* dest
;   )
;
x64LoadStackSegmentAndJump:
    lss     rax, [rcx]
    jmp QWORD [rdx]


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
;   OUT x64_seg_sel  *Gdtr
;   );
;
x64ReadGdtr:
    sgdt    [rcx]
    ret

;==================================
; VOID
; EFIAPI
; x64WriteGdtr (IN CONST x64_seg_sel *gdtr);
;
x64WriteGdtr:
    lgdt    [rcx]

    xor eax, eax
    ret

;==================================
; UINT16
; EFIAPI
; x64ReadCS (VOID);
;
x64ReadCS:
    xor eax, eax
    mov ax, cs
    ret

;==================================
; UINT16
; EFIAPI
; x64ReadDS (VOID);
;
x64ReadDS:
    xor eax, eax
    mov ax, ds
    ret

;==================================
; UINT16
; EFIAPI
; x64ReadSS (VOID);
;
x64ReadSS:
    xor eax, eax
    mov ax, ss
    ret

;==================================
; UINT16
; EFIAPI
; x64ReadES (VOID);
;
x64ReadES:
    xor eax, eax
    mov ax, es
    ret

;==================================
; UINT16
; EFIAPI
; x64ReadFS (VOID);
;
x64ReadFS:
    xor eax, eax
    mov ax, fs
    ret

;==================================
; UINT16
; EFIAPI
; x64ReadGS (VOID);
;
x64ReadGS:
    xor eax, eax
    mov ax, es
    ret
