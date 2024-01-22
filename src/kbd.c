#include "kbd.h"
#include "vga.h"
#define iret __asm__ ("leave;iret")

static uint8 kbd_map[128] =
{
    0, 27, /* Esc */
	'1', '2', '3', '4', '5', '6', '7', '8',	'9', '0', '-', '=', '\b',	/* Backspace */
    '\t',	/* Tab */
    'q', 'w', 'e', 'r',	't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',	/* Enter key */
    0,		/* 29 - Control */
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,		/* Left shift */
    '\\', 'z', 'x', 'c', 'v', 'b', 'n',	'm', ',', '.', '/', 0,			/* Right shift */
    '*',
    0,		/* Alt */
    ' ',	/* Space bar */
    0,		/* Caps lock */
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,	/* 59 - F1 ... F10 */
    0,		/* 69 - Num lock*/
    0,		/* Scroll Lock */
    0,		/* Home key */
    0,		/* Up Arrow */
    0,		/* Page Up */
    '-',
    0,		/* Left Arrow */
    0,
    0,		/* Right Arrow */
    '+',
    0,		/* 79 - End key*/
    0,		/* Down Arrow */
    0,		/* Page Down */
    0,		/* Insert Key */
    0,		/* Delete Key */
    0, 0, 0,
    0,		/* F11 Key */
    0,		/* F12 Key */
    0,		/* All other keys are undefined */
};

void kbd_init(void)
{
	/* ICW1 - begin initialization */
	write_port(PIC1_COMMAND_PORT , 0x11);
	write_port(PIC2_COMMAND_PORT , 0x11);

	/* ICW2 - remap offset address of IDT */
	/*
	* In x86 protected mode, we have to remap the PICs beyond 0x20 because
	* Intel have designated the first 32 interrupts as "reserved" for cpu exceptions
	*/
	write_port(PIC1_DATA_PORT , 0x20);
	write_port(PIC2_DATA_PORT , 0x28);

	/* ICW3 - setup cascading */
	write_port(PIC1_DATA_PORT , 0x00);
	write_port(PIC2_DATA_PORT , 0x00);

	/* ICW4 - environment info */
	write_port(PIC1_DATA_PORT , 0x01);
	write_port(PIC2_DATA_PORT , 0x01);
	/* Initialization finished */

	/* mask interrupts */
	write_port(PIC1_DATA_PORT , 0xff);
	write_port(PIC2_DATA_PORT , 0xff);

	/* 0xFD is 11111101 - enables only IRQ1 (keyboard)*/
	// write_port(PIC1_DATA_PORT , 0xFD);
	write_port(PIC1_DATA_PORT , 0xFC);
	//write_port(PIC1_DATA_PORT, read_port(PIC1_DATA_PORT) & 0xFE);
}

void kbd_interrupt_handler(void)
{
	uint8 status;
	char keycode;

	/* write EOI */
	write_port(PIC1_COMMAND_PORT, 0x20);

	status = read_port(KBD_STATUS_PORT);
	/* Lowest bit of status will be set if buffer is not empty */
	if (status & 0x01){	// key down
		keycode = read_port(KBD_DATA_PORT);
		if (keycode > 0){
			uint8 ch = kbd_map[(unsigned char) keycode];
			if (keycode == '\n')
				print("\n");
			else
				term_putchar(ch);
		}
	}
}

void kbd_interrupt(){
	kbd_interrupt_handler();
	iret;
}

