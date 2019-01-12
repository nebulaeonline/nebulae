; Copyright (c) 2018-2019 Nebulae Foundation. All rights reserved.
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
; 3. Neither the name of the copyright holder nor the names of its contributors 
; may be used to endorse or promote products derived from this software without 
; specific prior written permission.
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

    .code

;==================================
; VOID x64AsmOutportB(
;   UINT16 Port, 
;   UINT8  Value)
;
x64AsmOutportB PROC
    mov rax,rcx
    mov rbx,rdx

    mov dx,ax
    mov al,bl

    out dx,al

    xor eax,eax

    ret
x64AsmOutportB ENDP

;==================================
; VOID x64AsmOutportW(
;   UINT16 Port, 
;   UINT16 Value)
;
x64AsmOutportW PROC
    mov rax,rcx
    mov rbx,rdx

    mov dx,ax
    mov ax,bx

    out dx,ax

    xor eax,eax

    ret
x64AsmOutportW ENDP

;==================================
; UINT8 x64AsmInportB(
;   UINT16 Port)
;
x64AsmInportB PROC
    mov rax,rcx
    mov dx,ax

    in al,dx

    ret
x64AsmInportB ENDP

;==================================
; UINT16 x64AsmInportW(
;   UINT16 Port)
;
x64AsmInportW PROC
    mov rax,rcx
    mov dx,ax

    in eax,dx

    ret
x64AsmInportW ENDP

;==================================
; VOID x64EnableInterrupts()
;
x64EnableInterrupts PROC
    cli
    xor eax,eax
    ret
x64EnableInterrupts ENDP

;==================================
; VOID x64DisableInterrupts()
;
x64DisableInterrupts PROC
    sti
    xor eax,eax
    ret
x64DisableInterrupts ENDP


;==================================
; UINT64
; EFIAPI
; x64ReadTsc (
;   VOID
;   );
;------------------------------------------------------------------------------
x64ReadTsc  PROC
    rdtsc
    shl     rdx, 20h
    or      rax, rdx
    ret
x64ReadTsc  ENDP

;==================================
; x64_farptr*
; EFIAPI
; x64LoadStackSegment (
;   x64_farptr*
;   );
;------------------------------------------------------------------------------
x64LoadStackSegment  PROC
    lss     rax, [rcx]
    ret
x64LoadStackSegment  ENDP

;==================================
    END
