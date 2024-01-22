bits 32
section .text
	align	4
;	dd	0x1BADB002
;	dd	0x00
;	dd	- (0x1BADB002 + 0x00)

global start
;global stack_space

extern kernel

start2:
	cli
;	mov	esp, stack_space
	call	kernel

forever:
	hlt
	jmp	forever

section .bss
;	resb 16384
; stack_space: