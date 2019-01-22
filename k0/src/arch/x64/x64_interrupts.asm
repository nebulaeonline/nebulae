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

bits 64
segment .text

; Our C fns
extern IsrHandler, NMIHandler

; Exceptions/Faults/Traps
global interrupt_0x00_wrapper, interrupt_0x01_wrapper, interrupt_0x02_wrapper, interrupt_0x03_wrapper
global interrupt_0x04_wrapper, interrupt_0x05_wrapper, interrupt_0x06_wrapper, interrupt_0x07_wrapper
global interrupt_0x08_wrapper, interrupt_0x09_wrapper, interrupt_0x0A_wrapper, interrupt_0x0B_wrapper
global interrupt_0x0C_wrapper, interrupt_0x0D_wrapper, interrupt_0x0E_wrapper, interrupt_0x10_wrapper
global interrupt_0x11_wrapper, interrupt_0x12_wrapper, interrupt_0x13_wrapper, interrupt_0x14_wrapper

; Hardware / User-Defined Interrupts
global interrupt_0x20_wrapper, interrupt_0x21_wrapper, interrupt_0x22_wrapper, interrupt_0x23_wrapper
global interrupt_0x24_wrapper, interrupt_0x25_wrapper, interrupt_0x26_wrapper, interrupt_0x27_wrapper
global interrupt_0x28_wrapper, interrupt_0x29_wrapper, interrupt_0x2A_wrapper, interrupt_0x2B_wrapper
global interrupt_0x2C_wrapper, interrupt_0x2D_wrapper, interrupt_0x2E_wrapper, interrupt_0x2F_wrapper
global interrupt_0x30_wrapper, interrupt_0x31_wrapper, interrupt_0x32_wrapper, interrupt_0x33_wrapper
global interrupt_0x34_wrapper, interrupt_0x35_wrapper, interrupt_0x36_wrapper, interrupt_0x37_wrapper
global interrupt_0x38_wrapper, interrupt_0x39_wrapper, interrupt_0x3A_wrapper, interrupt_0x3B_wrapper
global interrupt_0x3C_wrapper, interrupt_0x3D_wrapper, interrupt_0x3E_wrapper, interrupt_0x3F_wrapper
global interrupt_0x40_wrapper, interrupt_0x41_wrapper, interrupt_0x42_wrapper, interrupt_0x43_wrapper
global interrupt_0x44_wrapper, interrupt_0x45_wrapper, interrupt_0x46_wrapper, interrupt_0x47_wrapper
global interrupt_0x48_wrapper, interrupt_0x49_wrapper, interrupt_0x4A_wrapper, interrupt_0x4B_wrapper
global interrupt_0x4C_wrapper, interrupt_0x4D_wrapper, interrupt_0x4E_wrapper, interrupt_0x4F_wrapper
global interrupt_0x50_wrapper, interrupt_0x51_wrapper, interrupt_0x52_wrapper, interrupt_0x53_wrapper
global interrupt_0x54_wrapper, interrupt_0x55_wrapper, interrupt_0x56_wrapper, interrupt_0x57_wrapper
global interrupt_0x58_wrapper, interrupt_0x59_wrapper, interrupt_0x5A_wrapper, interrupt_0x5B_wrapper
global interrupt_0x5C_wrapper, interrupt_0x5D_wrapper, interrupt_0x5E_wrapper, interrupt_0x5F_wrapper
global interrupt_0x60_wrapper, interrupt_0x61_wrapper, interrupt_0x62_wrapper, interrupt_0x63_wrapper
global interrupt_0x64_wrapper, interrupt_0x65_wrapper, interrupt_0x66_wrapper, interrupt_0x67_wrapper
global interrupt_0x68_wrapper, interrupt_0x69_wrapper, interrupt_0x6A_wrapper, interrupt_0x6B_wrapper
global interrupt_0x6C_wrapper, interrupt_0x6D_wrapper, interrupt_0x6E_wrapper, interrupt_0x6F_wrapper
global interrupt_0x70_wrapper, interrupt_0x71_wrapper, interrupt_0x72_wrapper, interrupt_0x73_wrapper
global interrupt_0x74_wrapper, interrupt_0x75_wrapper, interrupt_0x76_wrapper, interrupt_0x77_wrapper
global interrupt_0x78_wrapper, interrupt_0x79_wrapper, interrupt_0x7A_wrapper, interrupt_0x7B_wrapper
global interrupt_0x7C_wrapper, interrupt_0x7D_wrapper, interrupt_0x7E_wrapper, interrupt_0x7F_wrapper
global interrupt_0x80_wrapper, interrupt_0x81_wrapper, interrupt_0x82_wrapper, interrupt_0x83_wrapper
global interrupt_0x84_wrapper, interrupt_0x85_wrapper, interrupt_0x86_wrapper, interrupt_0x87_wrapper
global interrupt_0x88_wrapper, interrupt_0x89_wrapper, interrupt_0x8A_wrapper, interrupt_0x8B_wrapper
global interrupt_0x8C_wrapper, interrupt_0x8D_wrapper, interrupt_0x8E_wrapper, interrupt_0x8F_wrapper
global interrupt_0x90_wrapper, interrupt_0x91_wrapper, interrupt_0x92_wrapper, interrupt_0x93_wrapper
global interrupt_0x94_wrapper, interrupt_0x95_wrapper, interrupt_0x96_wrapper, interrupt_0x97_wrapper
global interrupt_0x98_wrapper, interrupt_0x99_wrapper, interrupt_0x9A_wrapper, interrupt_0x9B_wrapper
global interrupt_0x9C_wrapper, interrupt_0x9D_wrapper, interrupt_0x9E_wrapper, interrupt_0x9F_wrapper
global interrupt_0xA0_wrapper, interrupt_0xA1_wrapper, interrupt_0xA2_wrapper, interrupt_0xA3_wrapper
global interrupt_0xA4_wrapper, interrupt_0xA5_wrapper, interrupt_0xA6_wrapper, interrupt_0xA7_wrapper
global interrupt_0xA8_wrapper, interrupt_0xA9_wrapper, interrupt_0xAA_wrapper, interrupt_0xAB_wrapper
global interrupt_0xAC_wrapper, interrupt_0xAD_wrapper, interrupt_0xAE_wrapper, interrupt_0xAF_wrapper
global interrupt_0xB0_wrapper, interrupt_0xB1_wrapper, interrupt_0xB2_wrapper, interrupt_0xB3_wrapper
global interrupt_0xB4_wrapper, interrupt_0xB5_wrapper, interrupt_0xB6_wrapper, interrupt_0xB7_wrapper
global interrupt_0xB8_wrapper, interrupt_0xB9_wrapper, interrupt_0xBA_wrapper, interrupt_0xBB_wrapper
global interrupt_0xBC_wrapper, interrupt_0xBD_wrapper, interrupt_0xBE_wrapper, interrupt_0xBF_wrapper
global interrupt_0xC0_wrapper, interrupt_0xC1_wrapper, interrupt_0xC2_wrapper, interrupt_0xC3_wrapper
global interrupt_0xC4_wrapper, interrupt_0xC5_wrapper, interrupt_0xC6_wrapper, interrupt_0xC7_wrapper
global interrupt_0xC8_wrapper, interrupt_0xC9_wrapper, interrupt_0xCA_wrapper, interrupt_0xCB_wrapper
global interrupt_0xCC_wrapper, interrupt_0xCD_wrapper, interrupt_0xCE_wrapper, interrupt_0xCF_wrapper
global interrupt_0xD0_wrapper, interrupt_0xD1_wrapper, interrupt_0xD2_wrapper, interrupt_0xD3_wrapper
global interrupt_0xD4_wrapper, interrupt_0xD5_wrapper, interrupt_0xD6_wrapper, interrupt_0xD7_wrapper
global interrupt_0xD8_wrapper, interrupt_0xD9_wrapper, interrupt_0xDA_wrapper, interrupt_0xDB_wrapper
global interrupt_0xDC_wrapper, interrupt_0xDD_wrapper, interrupt_0xDE_wrapper, interrupt_0xDF_wrapper
global interrupt_0xE0_wrapper, interrupt_0xE1_wrapper, interrupt_0xE2_wrapper, interrupt_0xE3_wrapper
global interrupt_0xE4_wrapper, interrupt_0xE5_wrapper, interrupt_0xE6_wrapper, interrupt_0xE7_wrapper
global interrupt_0xE8_wrapper, interrupt_0xE9_wrapper, interrupt_0xEA_wrapper, interrupt_0xEB_wrapper
global interrupt_0xEC_wrapper, interrupt_0xED_wrapper, interrupt_0xEE_wrapper, interrupt_0xEF_wrapper
global interrupt_0xF0_wrapper, interrupt_0xF1_wrapper, interrupt_0xF2_wrapper, interrupt_0xF3_wrapper
global interrupt_0xF4_wrapper, interrupt_0xF5_wrapper, interrupt_0xF6_wrapper, interrupt_0xF7_wrapper
global interrupt_0xF8_wrapper, interrupt_0xF9_wrapper, interrupt_0xFA_wrapper, interrupt_0xFB_wrapper
global interrupt_0xFC_wrapper, interrupt_0xFD_wrapper, interrupt_0xFE_wrapper, interrupt_0xFF_wrapper

; nebulae segment register gdt index values
; DPL0_DATA_WRITEABLE             0x100
; DPL0_CODE_READABLE              0x108
; DPL0_CODE64_READABLE            0x110

; DPL3_DATA_WRITEABLE             0x118
; DPL3_CODE_READABLE              0x120
; DPL3_CODE64_READABLE            0x128

; CPU0_TSS_INDEX_GDT              0x130

%macro  isr_entrypoint 2
    push    rax
    push    rcx
    push    rdx
    push    rbx
    push    rsi
    push    rdi
    push    rbp
    push    r8
    push    r9
    push    r10
    push    r11
    push    r12
    push    r13
    push    r14
    push    r15

    ; floating point state
    ; allocate 512 bytes on the stack
    mov     rax, rsp
    sub     rsp, 512
    xsave64 [rax]

    xor     rax, rax
    mov     ax, ds
    push    rax

    xor     rax, rax
    mov     ax, es
    push    rax

    xor     rax, rax
    mov     ax, fs
    push    rax

    xor     rax, rax
    mov     ax, gs
    push    rax

	mov     ax, 0x100               ; DPL0_DATA_WRITEABLE
	mov     ds, ax
	mov     es, ax                  ; load segments with valid selector
	mov 	fs, ax
	mov 	gs, ax

	mov     rcx, %1                 ; Send the vector to the C isr handler

    push    rsp                     ; push two copies of rsp 
    push qword [rsp]
    and     rsp,-0x10               ; align the stack to 16 bytes
	call    %2                      ; call the specified c function
    mov     rsp, [rsp + 8]          ; restore our stack
    
	; restore the segment registers
    pop     rax                     ; return value - isrs return nothing, but do allocate
    pop     rax                     ; gs
    mov     gs, ax
    pop     rax                     ; fs
    mov     fs, ax              
    pop     rax                     ; es
    mov     es, ax
    pop     rax                     ; ds
    mov     ds, ax
    
    ; restore floating point state
    ; skip over the 512 byte xsave/xrstor
    add     rsp, 512
    xrstor64 [rsp]

    ; restore regular regs
    pop     r15
    pop     r14
    pop     r13
    pop     r12
    pop     r11
    pop     r10
    pop     r9
    pop     r8
    pop     rbp
    pop     rdi
    pop     rsi
    pop     rbx
    pop     rdx
    pop     rcx
    pop     rax

    iretq

%endmacro

; Exception 0x00 (0) - Divide Error
interrupt_0x00_wrapper:
    isr_entrypoint 0x00, IsrHandler

; Exception 0x01 (1) - Debug Exception	
interrupt_0x01_wrapper:
	isr_entrypoint 0x01, IsrHandler

; Interrupt 0x02 (2) - NMI Interrupt
interrupt_0x02_wrapper:
    isr_entrypoint 0x02, NMIHandler

; Exception 0x03 (3) - Breakpoint
interrupt_0x03_wrapper:
	isr_entrypoint 0x03, IsrHandler

; Exception 0x04 (4) - Overflow
interrupt_0x04_wrapper:
	isr_entrypoint 0x04, IsrHandler
	
; Exception 0x05 (5) - Bound Range Exceeded
interrupt_0x05_wrapper:
	isr_entrypoint 0x05, IsrHandler

; Exception 0x06 (6) - Invalid/Undefined Opcode
interrupt_0x06_wrapper:
    isr_entrypoint 0x06, IsrHandler

; Exception 0x07 (7) - No Math Coprocessor
interrupt_0x07_wrapper:
	isr_entrypoint 0x07, IsrHandler
	
; Exception 0x08 (8) - Double Fault
interrupt_0x08_wrapper:
	isr_entrypoint 0x08, IsrHandler
	
; Exception 0x09 (9) - Coprocessor Segment Overrun (Reserved)
interrupt_0x09_wrapper:
	isr_entrypoint 0x09, IsrHandler
	
; Exception 0x0A (10) - Invalid TSS
interrupt_0x0A_wrapper:
	isr_entrypoint 0x0A, IsrHandler

; Exception 0x0B (11) - Segment Not Present	
interrupt_0x0B_wrapper:
	isr_entrypoint 0x0B, IsrHandler

; Exception 0x0C (12) - Stack-Segment Fault
interrupt_0x0C_wrapper:
	isr_entrypoint 0x0C, IsrHandler
	
; Exception 0x0D (13) - General Protection Fault
interrupt_0x0D_wrapper:
	isr_entrypoint 0x0D, IsrHandler
	
; Exception 0x0E (14) - Page Fault
interrupt_0x0E_wrapper:
	isr_entrypoint 0x0E, IsrHandler

; Exception 0x10 (16) - x86 Floating-Point Error	
interrupt_0x10_wrapper:
	isr_entrypoint 0x10, IsrHandler
	
; Exception 0x11 (17) - Alignment Check
interrupt_0x11_wrapper:
	isr_entrypoint 0x11, IsrHandler
	
; Exception 0x12 (18) - Machine Check
interrupt_0x12_wrapper:
	isr_entrypoint 0x12, IsrHandler
	
; Exception 0x13 (19) - SIMD Floating-Point
interrupt_0x13_wrapper:
	isr_entrypoint 0x13, IsrHandler

; Exception 0x14 (20) - Virtualization Exception
interrupt_0x14_wrapper:
	isr_entrypoint 0x14, IsrHandler

; Interrupt 0x20 (32) - INT32
interrupt_0x20_wrapper:
    isr_entrypoint 0x20, IsrHandler

; Interrupt 0x21 (33) - INT33
interrupt_0x21_wrapper:
    isr_entrypoint 0x21, IsrHandler

; Interrupt 0x22 (34) - INT34
interrupt_0x22_wrapper:
    isr_entrypoint 0x22, IsrHandler

; Interrupt 0x23 (35) - INT35
interrupt_0x23_wrapper:
    isr_entrypoint 0x23, IsrHandler

; Interrupt 0x24 (36) - INT36
interrupt_0x24_wrapper:
    isr_entrypoint 0x24, IsrHandler

; Interrupt 0x25 (37) - INT37
interrupt_0x25_wrapper:
    isr_entrypoint 0x25, IsrHandler

; Interrupt 0x26 (38) - INT38
interrupt_0x26_wrapper:
    isr_entrypoint 0x26, IsrHandler

; Interrupt 0x27 (39) - INT39
interrupt_0x27_wrapper:
    isr_entrypoint 0x27, IsrHandler

; Interrupt 0x28 (40) - INT40
interrupt_0x28_wrapper:
    isr_entrypoint 0x28, IsrHandler

; Interrupt 0x29 (41) - INT41
interrupt_0x29_wrapper:
    isr_entrypoint 0x29, IsrHandler

; Interrupt 0x2A (42) - INT42
interrupt_0x2A_wrapper:
    isr_entrypoint 0x2A, IsrHandler

; Interrupt 0x2B (43) - INT43
interrupt_0x2B_wrapper:
    isr_entrypoint 0x2B, IsrHandler

; Interrupt 0x2C (44) - INT44
interrupt_0x2C_wrapper:
    isr_entrypoint 0x2C, IsrHandler

; Interrupt 0x2D (45) - INT45
interrupt_0x2D_wrapper:
    isr_entrypoint 0x2D, IsrHandler

; Interrupt 0x2E (46) - INT46
interrupt_0x2E_wrapper:
    isr_entrypoint 0x2E, IsrHandler

; Interrupt 0x2F (47) - INT47
interrupt_0x2F_wrapper:
    isr_entrypoint 0x2F, IsrHandler

; Interrupt 0x30 (48) - INT48
interrupt_0x30_wrapper:
    isr_entrypoint 0x30, IsrHandler

; Interrupt 0x31 (49) - INT49
interrupt_0x31_wrapper:
    isr_entrypoint 0x31, IsrHandler

; Interrupt 0x32 (50) - INT50
interrupt_0x32_wrapper:
    isr_entrypoint 0x32, IsrHandler

; Interrupt 0x33 (51) - INT51
interrupt_0x33_wrapper:
    isr_entrypoint 0x33, IsrHandler

; Interrupt 0x34 (52) - INT52
interrupt_0x34_wrapper:
    isr_entrypoint 0x34, IsrHandler

; Interrupt 0x35 (53) - INT53
interrupt_0x35_wrapper:
    isr_entrypoint 0x35, IsrHandler

; Interrupt 0x36 (54) - INT54
interrupt_0x36_wrapper:
    isr_entrypoint 0x36, IsrHandler

; Interrupt 0x37 (55) - INT55
interrupt_0x37_wrapper:
    isr_entrypoint 0x37, IsrHandler

; Interrupt 0x38 (56) - INT56
interrupt_0x38_wrapper:
    isr_entrypoint 0x38, IsrHandler

; Interrupt 0x39 (57) - INT57
interrupt_0x39_wrapper:
    isr_entrypoint 0x39, IsrHandler

; Interrupt 0x3A (58) - INT58
interrupt_0x3A_wrapper:
    isr_entrypoint 0x3A, IsrHandler

; Interrupt 0x3B (59) - INT59
interrupt_0x3B_wrapper:
    isr_entrypoint 0x3B, IsrHandler

; Interrupt 0x3C (60) - INT60
interrupt_0x3C_wrapper:
    isr_entrypoint 0x3C, IsrHandler

; Interrupt 0x3D (61) - INT61
interrupt_0x3D_wrapper:
    isr_entrypoint 0x3D, IsrHandler

; Interrupt 0x3E (62) - INT62
interrupt_0x3E_wrapper:
    isr_entrypoint 0x3E, IsrHandler

; Interrupt 0x3F (63) - INT63
interrupt_0x3F_wrapper:
    isr_entrypoint 0x3F, IsrHandler

; Interrupt 0x40 (64) - INT64
interrupt_0x40_wrapper:
    isr_entrypoint 0x40, IsrHandler

; Interrupt 0x41 (65) - INT65
interrupt_0x41_wrapper:
    isr_entrypoint 0x41, IsrHandler

; Interrupt 0x42 (66) - INT66
interrupt_0x42_wrapper:
    isr_entrypoint 0x42, IsrHandler

; Interrupt 0x43 (67) - INT67
interrupt_0x43_wrapper:
    isr_entrypoint 0x43, IsrHandler

; Interrupt 0x44 (68) - INT68
interrupt_0x44_wrapper:
    isr_entrypoint 0x44, IsrHandler

; Interrupt 0x45 (69) - INT69
interrupt_0x45_wrapper:
    isr_entrypoint 0x45, IsrHandler

; Interrupt 0x46 (70) - INT70
interrupt_0x46_wrapper:
    isr_entrypoint 0x46, IsrHandler

; Interrupt 0x47 (71) - INT71
interrupt_0x47_wrapper:
    isr_entrypoint 0x47, IsrHandler

; Interrupt 0x48 (72) - INT72
interrupt_0x48_wrapper:
    isr_entrypoint 0x48, IsrHandler

; Interrupt 0x49 (73) - INT73
interrupt_0x49_wrapper:
    isr_entrypoint 0x49, IsrHandler

; Interrupt 0x4A (74) - INT74
interrupt_0x4A_wrapper:
    isr_entrypoint 0x4A, IsrHandler

; Interrupt 0x4B (75) - INT75
interrupt_0x4B_wrapper:
    isr_entrypoint 0x4B, IsrHandler

; Interrupt 0x4C (76) - INT76
interrupt_0x4C_wrapper:
    isr_entrypoint 0x4C, IsrHandler

; Interrupt 0x4D (77) - INT77
interrupt_0x4D_wrapper:
    isr_entrypoint 0x4D, IsrHandler

; Interrupt 0x4E (78) - INT78
interrupt_0x4E_wrapper:
    isr_entrypoint 0x4E, IsrHandler

; Interrupt 0x4F (79) - INT79
interrupt_0x4F_wrapper:
    isr_entrypoint 0x4F, IsrHandler

; Interrupt 0x50 (80) - INT80
interrupt_0x50_wrapper:
    isr_entrypoint 0x50, IsrHandler

; Interrupt 0x51 (81) - INT81
interrupt_0x51_wrapper:
    isr_entrypoint 0x51, IsrHandler

; Interrupt 0x52 (82) - INT82
interrupt_0x52_wrapper:
    isr_entrypoint 0x52, IsrHandler

; Interrupt 0x53 (83) - INT83
interrupt_0x53_wrapper:
    isr_entrypoint 0x53, IsrHandler

; Interrupt 0x54 (84) - INT84
interrupt_0x54_wrapper:
    isr_entrypoint 0x54, IsrHandler

; Interrupt 0x55 (85) - INT85
interrupt_0x55_wrapper:
    isr_entrypoint 0x55, IsrHandler

; Interrupt 0x56 (86) - INT86
interrupt_0x56_wrapper:
    isr_entrypoint 0x56, IsrHandler

; Interrupt 0x57 (87) - INT87
interrupt_0x57_wrapper:
    isr_entrypoint 0x57, IsrHandler

; Interrupt 0x58 (88) - INT88
interrupt_0x58_wrapper:
    isr_entrypoint 0x58, IsrHandler

; Interrupt 0x59 (89) - INT89
interrupt_0x59_wrapper:
    isr_entrypoint 0x59, IsrHandler

; Interrupt 0x5A (90) - INT90
interrupt_0x5A_wrapper:
    isr_entrypoint 0x5A, IsrHandler

; Interrupt 0x5B (91) - INT91
interrupt_0x5B_wrapper:
    isr_entrypoint 0x5B, IsrHandler

; Interrupt 0x5C (92) - INT92
interrupt_0x5C_wrapper:
    isr_entrypoint 0x5C, IsrHandler

; Interrupt 0x5D (93) - INT93
interrupt_0x5D_wrapper:
    isr_entrypoint 0x5D, IsrHandler

; Interrupt 0x5E (94) - INT94
interrupt_0x5E_wrapper:
    isr_entrypoint 0x5E, IsrHandler

; Interrupt 0x5F (95) - INT95
interrupt_0x5F_wrapper:
    isr_entrypoint 0x5F, IsrHandler

; Interrupt 0x60 (96) - INT96
interrupt_0x60_wrapper:
    isr_entrypoint 0x60, IsrHandler

; Interrupt 0x61 (97) - INT97
interrupt_0x61_wrapper:
    isr_entrypoint 0x61, IsrHandler

; Interrupt 0x62 (98) - INT98
interrupt_0x62_wrapper:
    isr_entrypoint 0x62, IsrHandler

; Interrupt 0x63 (99) - INT99
interrupt_0x63_wrapper:
    isr_entrypoint 0x63, IsrHandler

; Interrupt 0x64 (100) - INT100
interrupt_0x64_wrapper:
    isr_entrypoint 0x64, IsrHandler

; Interrupt 0x65 (101) - INT101
interrupt_0x65_wrapper:
    isr_entrypoint 0x65, IsrHandler

; Interrupt 0x66 (102) - INT102
interrupt_0x66_wrapper:
    isr_entrypoint 0x66, IsrHandler

; Interrupt 0x67 (103) - INT103
interrupt_0x67_wrapper:
    isr_entrypoint 0x67, IsrHandler

; Interrupt 0x68 (104) - INT104
interrupt_0x68_wrapper:
    isr_entrypoint 0x68, IsrHandler

; Interrupt 0x69 (105) - INT105
interrupt_0x69_wrapper:
    isr_entrypoint 0x69, IsrHandler

; Interrupt 0x6A (106) - INT106
interrupt_0x6A_wrapper:
    isr_entrypoint 0x6A, IsrHandler

; Interrupt 0x6B (107) - INT107
interrupt_0x6B_wrapper:
    isr_entrypoint 0x6B, IsrHandler

; Interrupt 0x6C (108) - INT108
interrupt_0x6C_wrapper:
    isr_entrypoint 0x6C, IsrHandler

; Interrupt 0x6D (109) - INT109
interrupt_0x6D_wrapper:
    isr_entrypoint 0x6D, IsrHandler

; Interrupt 0x6E (110) - INT110
interrupt_0x6E_wrapper:
    isr_entrypoint 0x6E, IsrHandler

; Interrupt 0x6F (111) - INT111
interrupt_0x6F_wrapper:
    isr_entrypoint 0x6F, IsrHandler

; Interrupt 0x70 (112) - INT112
interrupt_0x70_wrapper:
    isr_entrypoint 0x70, IsrHandler

; Interrupt 0x71 (113) - INT113
interrupt_0x71_wrapper:
    isr_entrypoint 0x71, IsrHandler

; Interrupt 0x72 (114) - INT114
interrupt_0x72_wrapper:
    isr_entrypoint 0x72, IsrHandler

; Interrupt 0x73 (115) - INT115
interrupt_0x73_wrapper:
    isr_entrypoint 0x73, IsrHandler

; Interrupt 0x74 (116) - INT116
interrupt_0x74_wrapper:
    isr_entrypoint 0x74, IsrHandler

; Interrupt 0x75 (117) - INT117
interrupt_0x75_wrapper:
    isr_entrypoint 0x75, IsrHandler

; Interrupt 0x76 (118) - INT118
interrupt_0x76_wrapper:
    isr_entrypoint 0x76, IsrHandler

; Interrupt 0x77 (119) - INT119
interrupt_0x77_wrapper:
    isr_entrypoint 0x77, IsrHandler

; Interrupt 0x78 (120) - INT120
interrupt_0x78_wrapper:
    isr_entrypoint 0x78, IsrHandler

; Interrupt 0x79 (121) - INT121
interrupt_0x79_wrapper:
    isr_entrypoint 0x79, IsrHandler

; Interrupt 0x7A (122) - INT122
interrupt_0x7A_wrapper:
    isr_entrypoint 0x7A, IsrHandler

; Interrupt 0x7B (123) - INT123
interrupt_0x7B_wrapper:
    isr_entrypoint 0x7B, IsrHandler

; Interrupt 0x7C (124) - INT124
interrupt_0x7C_wrapper:
    isr_entrypoint 0x7C, IsrHandler

; Interrupt 0x7D (125) - INT125
interrupt_0x7D_wrapper:
    isr_entrypoint 0x7D, IsrHandler

; Interrupt 0x7E (126) - INT126
interrupt_0x7E_wrapper:
    isr_entrypoint 0x7E, IsrHandler

; Interrupt 0x7F (127) - INT127
interrupt_0x7F_wrapper:
    isr_entrypoint 0x7F, IsrHandler

; Interrupt 0x80 (128) - INT128
interrupt_0x80_wrapper:
    isr_entrypoint 0x80, IsrHandler

; Interrupt 0x81 (129) - INT129
interrupt_0x81_wrapper:
    isr_entrypoint 0x81, IsrHandler

; Interrupt 0x82 (130) - INT130
interrupt_0x82_wrapper:
    isr_entrypoint 0x82, IsrHandler

; Interrupt 0x83 (131) - INT131
interrupt_0x83_wrapper:
    isr_entrypoint 0x83, IsrHandler

; Interrupt 0x84 (132) - INT132
interrupt_0x84_wrapper:
    isr_entrypoint 0x84, IsrHandler

; Interrupt 0x85 (133) - INT133
interrupt_0x85_wrapper:
    isr_entrypoint 0x85, IsrHandler

; Interrupt 0x86 (134) - INT134
interrupt_0x86_wrapper:
    isr_entrypoint 0x86, IsrHandler

; Interrupt 0x87 (135) - INT135
interrupt_0x87_wrapper:
    isr_entrypoint 0x87, IsrHandler

; Interrupt 0x88 (136) - INT136
interrupt_0x88_wrapper:
    isr_entrypoint 0x88, IsrHandler

; Interrupt 0x89 (137) - INT137
interrupt_0x89_wrapper:
    isr_entrypoint 0x89, IsrHandler

; Interrupt 0x8A (138) - INT138
interrupt_0x8A_wrapper:
    isr_entrypoint 0x8A, IsrHandler

; Interrupt 0x8B (139) - INT139
interrupt_0x8B_wrapper:
    isr_entrypoint 0x8B, IsrHandler

; Interrupt 0x8C (140) - INT140
interrupt_0x8C_wrapper:
    isr_entrypoint 0x8C, IsrHandler

; Interrupt 0x8D (141) - INT141
interrupt_0x8D_wrapper:
    isr_entrypoint 0x8D, IsrHandler

; Interrupt 0x8E (142) - INT142
interrupt_0x8E_wrapper:
    isr_entrypoint 0x8E, IsrHandler

; Interrupt 0x8F (143) - INT143
interrupt_0x8F_wrapper:
    isr_entrypoint 0x8F, IsrHandler

; Interrupt 0x90 (144) - INT144
interrupt_0x90_wrapper:
    isr_entrypoint 0x90, IsrHandler

; Interrupt 0x91 (145) - INT145
interrupt_0x91_wrapper:
    isr_entrypoint 0x91, IsrHandler

; Interrupt 0x92 (146) - INT146
interrupt_0x92_wrapper:
    isr_entrypoint 0x92, IsrHandler

; Interrupt 0x93 (147) - INT147
interrupt_0x93_wrapper:
    isr_entrypoint 0x93, IsrHandler

; Interrupt 0x94 (148) - INT148
interrupt_0x94_wrapper:
    isr_entrypoint 0x94, IsrHandler

; Interrupt 0x95 (149) - INT149
interrupt_0x95_wrapper:
    isr_entrypoint 0x95, IsrHandler

; Interrupt 0x96 (150) - INT150
interrupt_0x96_wrapper:
    isr_entrypoint 0x96, IsrHandler

; Interrupt 0x97 (151) - INT151
interrupt_0x97_wrapper:
    isr_entrypoint 0x97, IsrHandler

; Interrupt 0x98 (152) - INT152
interrupt_0x98_wrapper:
    isr_entrypoint 0x98, IsrHandler

; Interrupt 0x99 (153) - INT153
interrupt_0x99_wrapper:
    isr_entrypoint 0x99, IsrHandler

; Interrupt 0x9A (154) - INT154
interrupt_0x9A_wrapper:
    isr_entrypoint 0x9A, IsrHandler

; Interrupt 0x9B (155) - INT155
interrupt_0x9B_wrapper:
    isr_entrypoint 0x9B, IsrHandler

; Interrupt 0x9C (156) - INT156
interrupt_0x9C_wrapper:
    isr_entrypoint 0x9C, IsrHandler

; Interrupt 0x9D (157) - INT157
interrupt_0x9D_wrapper:
    isr_entrypoint 0x9D, IsrHandler

; Interrupt 0x9E (158) - INT158
interrupt_0x9E_wrapper:
    isr_entrypoint 0x9E, IsrHandler

; Interrupt 0x9F (159) - INT159
interrupt_0x9F_wrapper:
    isr_entrypoint 0x9F, IsrHandler

; Interrupt 0xA0 (160) - INT160
interrupt_0xA0_wrapper:
    isr_entrypoint 0xA0, IsrHandler

; Interrupt 0xA1 (161) - INT161
interrupt_0xA1_wrapper:
    isr_entrypoint 0xA1, IsrHandler

; Interrupt 0xA2 (162) - INT162
interrupt_0xA2_wrapper:
    isr_entrypoint 0xA2, IsrHandler

; Interrupt 0xA3 (163) - INT163
interrupt_0xA3_wrapper:
    isr_entrypoint 0xA3, IsrHandler

; Interrupt 0xA4 (164) - INT164
interrupt_0xA4_wrapper:
    isr_entrypoint 0xA4, IsrHandler

; Interrupt 0xA5 (165) - INT165
interrupt_0xA5_wrapper:
    isr_entrypoint 0xA5, IsrHandler

; Interrupt 0xA6 (166) - INT166
interrupt_0xA6_wrapper:
    isr_entrypoint 0xA6, IsrHandler

; Interrupt 0xA7 (167) - INT167
interrupt_0xA7_wrapper:
    isr_entrypoint 0xA7, IsrHandler

; Interrupt 0xA8 (168) - INT168
interrupt_0xA8_wrapper:
    isr_entrypoint 0xA8, IsrHandler

; Interrupt 0xA9 (169) - INT169
interrupt_0xA9_wrapper:
    isr_entrypoint 0xA9, IsrHandler

; Interrupt 0xAA (170) - INT170
interrupt_0xAA_wrapper:
    isr_entrypoint 0xAA, IsrHandler

; Interrupt 0xAB (171) - INT171
interrupt_0xAB_wrapper:
    isr_entrypoint 0xAB, IsrHandler

; Interrupt 0xAC (172) - INT172
interrupt_0xAC_wrapper:
    isr_entrypoint 0xAC, IsrHandler

; Interrupt 0xAD (173) - INT173
interrupt_0xAD_wrapper:
    isr_entrypoint 0xAD, IsrHandler

; Interrupt 0xAE (174) - INT174
interrupt_0xAE_wrapper:
    isr_entrypoint 0xAE, IsrHandler

; Interrupt 0xAF (175) - INT175
interrupt_0xAF_wrapper:
    isr_entrypoint 0xAF, IsrHandler

; Interrupt 0xB0 (176) - INT176
interrupt_0xB0_wrapper:
    isr_entrypoint 0xB0, IsrHandler

; Interrupt 0xB1 (177) - INT177
interrupt_0xB1_wrapper:
    isr_entrypoint 0xB1, IsrHandler

; Interrupt 0xB2 (178) - INT178
interrupt_0xB2_wrapper:
    isr_entrypoint 0xB2, IsrHandler

; Interrupt 0xB3 (179) - INT179
interrupt_0xB3_wrapper:
    isr_entrypoint 0xB3, IsrHandler

; Interrupt 0xB4 (180) - INT180
interrupt_0xB4_wrapper:
    isr_entrypoint 0xB4, IsrHandler

; Interrupt 0xB5 (181) - INT181
interrupt_0xB5_wrapper:
    isr_entrypoint 0xB5, IsrHandler

; Interrupt 0xB6 (182) - INT182
interrupt_0xB6_wrapper:
    isr_entrypoint 0xB6, IsrHandler

; Interrupt 0xB7 (183) - INT183
interrupt_0xB7_wrapper:
    isr_entrypoint 0xB7, IsrHandler

; Interrupt 0xB8 (184) - INT184
interrupt_0xB8_wrapper:
    isr_entrypoint 0xB8, IsrHandler

; Interrupt 0xB9 (185) - INT185
interrupt_0xB9_wrapper:
    isr_entrypoint 0xB9, IsrHandler

; Interrupt 0xBA (186) - INT186
interrupt_0xBA_wrapper:
    isr_entrypoint 0xBA, IsrHandler

; Interrupt 0xBB (187) - INT187
interrupt_0xBB_wrapper:
    isr_entrypoint 0xBB, IsrHandler

; Interrupt 0xBC (188) - INT188
interrupt_0xBC_wrapper:
    isr_entrypoint 0xBC, IsrHandler

; Interrupt 0xBD (189) - INT189
interrupt_0xBD_wrapper:
    isr_entrypoint 0xBD, IsrHandler

; Interrupt 0xBE (190) - INT190
interrupt_0xBE_wrapper:
    isr_entrypoint 0xBE, IsrHandler

; Interrupt 0xBF (191) - INT191
interrupt_0xBF_wrapper:
    isr_entrypoint 0xBF, IsrHandler

; Interrupt 0xC0 (192) - INT192
interrupt_0xC0_wrapper:
    isr_entrypoint 0xC0, IsrHandler

; Interrupt 0xC1 (193) - INT193
interrupt_0xC1_wrapper:
    isr_entrypoint 0xC1, IsrHandler

; Interrupt 0xC2 (194) - INT194
interrupt_0xC2_wrapper:
    isr_entrypoint 0xC2, IsrHandler

; Interrupt 0xC3 (195) - INT195
interrupt_0xC3_wrapper:
    isr_entrypoint 0xC3, IsrHandler

; Interrupt 0xC4 (196) - INT196
interrupt_0xC4_wrapper:
    isr_entrypoint 0xC4, IsrHandler

; Interrupt 0xC5 (197) - INT197
interrupt_0xC5_wrapper:
    isr_entrypoint 0xC5, IsrHandler

; Interrupt 0xC6 (198) - INT198
interrupt_0xC6_wrapper:
    isr_entrypoint 0xC6, IsrHandler

; Interrupt 0xC7 (199) - INT199
interrupt_0xC7_wrapper:
    isr_entrypoint 0xC7, IsrHandler

; Interrupt 0xC8 (200) - INT200
interrupt_0xC8_wrapper:
    isr_entrypoint 0xC8, IsrHandler

; Interrupt 0xC9 (201) - INT201
interrupt_0xC9_wrapper:
    isr_entrypoint 0xC9, IsrHandler

; Interrupt 0xCA (202) - INT202
interrupt_0xCA_wrapper:
    isr_entrypoint 0xCA, IsrHandler

; Interrupt 0xCB (203) - INT203
interrupt_0xCB_wrapper:
    isr_entrypoint 0xCB, IsrHandler

; Interrupt 0xCC (204) - INT204
interrupt_0xCC_wrapper:
    isr_entrypoint 0xCC, IsrHandler

; Interrupt 0xCD (205) - INT205
interrupt_0xCD_wrapper:
    isr_entrypoint 0xCD, IsrHandler

; Interrupt 0xCE (206) - INT206
interrupt_0xCE_wrapper:
    isr_entrypoint 0xCE, IsrHandler

; Interrupt 0xCF (207) - INT207
interrupt_0xCF_wrapper:
    isr_entrypoint 0xCF, IsrHandler

; Interrupt 0xD0 (208) - INT208
interrupt_0xD0_wrapper:
    isr_entrypoint 0xD0, IsrHandler

; Interrupt 0xD1 (209) - INT209
interrupt_0xD1_wrapper:
    isr_entrypoint 0xD1, IsrHandler

; Interrupt 0xD2 (210) - INT210
interrupt_0xD2_wrapper:
    isr_entrypoint 0xD2, IsrHandler

; Interrupt 0xD3 (211) - INT211
interrupt_0xD3_wrapper:
    isr_entrypoint 0xD3, IsrHandler

; Interrupt 0xD4 (212) - INT212
interrupt_0xD4_wrapper:
    isr_entrypoint 0xD4, IsrHandler

; Interrupt 0xD5 (213) - INT213
interrupt_0xD5_wrapper:
    isr_entrypoint 0xD5, IsrHandler

; Interrupt 0xD6 (214) - INT214
interrupt_0xD6_wrapper:
    isr_entrypoint 0xD6, IsrHandler

; Interrupt 0xD7 (215) - INT215
interrupt_0xD7_wrapper:
    isr_entrypoint 0xD7, IsrHandler

; Interrupt 0xD8 (216) - INT216
interrupt_0xD8_wrapper:
    isr_entrypoint 0xD8, IsrHandler

; Interrupt 0xD9 (217) - INT217
interrupt_0xD9_wrapper:
    isr_entrypoint 0xD9, IsrHandler

; Interrupt 0xDA (218) - INT218
interrupt_0xDA_wrapper:
    isr_entrypoint 0xDA, IsrHandler

; Interrupt 0xDB (219) - INT219
interrupt_0xDB_wrapper:
    isr_entrypoint 0xDB, IsrHandler

; Interrupt 0xDC (220) - INT220
interrupt_0xDC_wrapper:
    isr_entrypoint 0xDC, IsrHandler

; Interrupt 0xDD (221) - INT221
interrupt_0xDD_wrapper:
    isr_entrypoint 0xDD, IsrHandler

; Interrupt 0xDE (222) - INT222
interrupt_0xDE_wrapper:
    isr_entrypoint 0xDE, IsrHandler

; Interrupt 0xDF (223) - INT223
interrupt_0xDF_wrapper:
    isr_entrypoint 0xDF, IsrHandler

; Interrupt 0xE0 (224) - INT224
interrupt_0xE0_wrapper:
    isr_entrypoint 0xE0, IsrHandler

; Interrupt 0xE1 (225) - INT225
interrupt_0xE1_wrapper:
    isr_entrypoint 0xE1, IsrHandler

; Interrupt 0xE2 (226) - INT226
interrupt_0xE2_wrapper:
    isr_entrypoint 0xE2, IsrHandler

; Interrupt 0xE3 (227) - INT227
interrupt_0xE3_wrapper:
    isr_entrypoint 0xE3, IsrHandler

; Interrupt 0xE4 (228) - INT228
interrupt_0xE4_wrapper:
    isr_entrypoint 0xE4, IsrHandler

; Interrupt 0xE5 (229) - INT229
interrupt_0xE5_wrapper:
    isr_entrypoint 0xE5, IsrHandler

; Interrupt 0xE6 (230) - INT230
interrupt_0xE6_wrapper:
    isr_entrypoint 0xE6, IsrHandler

; Interrupt 0xE7 (231) - INT231
interrupt_0xE7_wrapper:
    isr_entrypoint 0xE7, IsrHandler

; Interrupt 0xE8 (232) - INT232
interrupt_0xE8_wrapper:
    isr_entrypoint 0xE8, IsrHandler

; Interrupt 0xE9 (233) - INT233
interrupt_0xE9_wrapper:
    isr_entrypoint 0xE9, IsrHandler

; Interrupt 0xEA (234) - INT234
interrupt_0xEA_wrapper:
    isr_entrypoint 0xEA, IsrHandler

; Interrupt 0xEB (235) - INT235
interrupt_0xEB_wrapper:
    isr_entrypoint 0xEB, IsrHandler

; Interrupt 0xEC (236) - INT236
interrupt_0xEC_wrapper:
    isr_entrypoint 0xEC, IsrHandler

; Interrupt 0xED (237) - INT237
interrupt_0xED_wrapper:
    isr_entrypoint 0xED, IsrHandler

; Interrupt 0xEE (238) - INT238
interrupt_0xEE_wrapper:
    isr_entrypoint 0xEE, IsrHandler

; Interrupt 0xEF (239) - INT239
interrupt_0xEF_wrapper:
    isr_entrypoint 0xEF, IsrHandler

; Interrupt 0xF0 (240) - INT240
interrupt_0xF0_wrapper:
    isr_entrypoint 0xF0, IsrHandler

; Interrupt 0xF1 (241) - INT241
interrupt_0xF1_wrapper:
    isr_entrypoint 0xF1, IsrHandler

; Interrupt 0xF2 (242) - INT242
interrupt_0xF2_wrapper:
    isr_entrypoint 0xF2, IsrHandler

; Interrupt 0xF3 (243) - INT243
interrupt_0xF3_wrapper:
    isr_entrypoint 0xF3, IsrHandler

; Interrupt 0xF4 (244) - INT244
interrupt_0xF4_wrapper:
    isr_entrypoint 0xF4, IsrHandler

; Interrupt 0xF5 (245) - INT245
interrupt_0xF5_wrapper:
    isr_entrypoint 0xF5, IsrHandler

; Interrupt 0xF6 (246) - INT246
interrupt_0xF6_wrapper:
    isr_entrypoint 0xF6, IsrHandler

; Interrupt 0xF7 (247) - INT247
interrupt_0xF7_wrapper:
    isr_entrypoint 0xF7, IsrHandler

; Interrupt 0xF8 (248) - INT248
interrupt_0xF8_wrapper:
    isr_entrypoint 0xF8, IsrHandler

; Interrupt 0xF9 (249) - INT249
interrupt_0xF9_wrapper:
    isr_entrypoint 0xF9, IsrHandler

; Interrupt 0xFA (250) - INT250
interrupt_0xFA_wrapper:
    isr_entrypoint 0xFA, IsrHandler

; Interrupt 0xFB (251) - INT251
interrupt_0xFB_wrapper:
    isr_entrypoint 0xFB, IsrHandler

; Interrupt 0xFC (252) - INT252
interrupt_0xFC_wrapper:
    isr_entrypoint 0xFC, IsrHandler

; Interrupt 0xFD (253) - INT253
interrupt_0xFD_wrapper:
    isr_entrypoint 0xFD, IsrHandler

; Interrupt 0xFE (254) - INT254
interrupt_0xFE_wrapper:
    isr_entrypoint 0xFE, IsrHandler

; Interrupt 0xFF (255) - INT255
interrupt_0xFF_wrapper:
    isr_entrypoint 0xFF, IsrHandler
