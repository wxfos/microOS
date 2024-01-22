bits 32

global kbd_handler
global read_port
global write_port
global write_port16
global write_port32
global load_idt
global outb

extern kbd_interrupter_handler

read_port2:
	mov		edx, [esp + 4]
	in		al, dx
	ret

write_port2:
	mov		edx, [esp + 4]
	mov		al, [esp + 8]
	out		dx, al
	ret

write_port16:
	mov		edx, [esp + 4]
	mov		ax, [esp + 8]
	out		dx, ax
	ret

write_port32:
	mov		edx, [esp + 4]
	mov		eax, [esp + 8]
	out		dx, eax
	ret

load_idt2:
	mov		edx, [esp + 4]
	lidt	[edx]
	sti
	ret

kbd_handler:
	call	kbd_interrupter_handler
	iretd

outb:
	mov dx, [esp + 4]
	mov al, [esp + 8]
	out dx, al
	ret