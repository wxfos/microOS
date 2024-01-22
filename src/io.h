#ifndef INCLUDE_IO_H
#define INCLUDE_IO_H
#include "kdef.h"

#define PIC1_COMMAND_PORT           0x20
#define PIC1_DATA_PORT              0x21
#define PIC2_COMMAND_PORT           0xa0
#define PIC2_DATA_PORT              0xa1

#define KBD_DATA_PORT               0x60
#define KBD_STATUS_PORT             0x64
#define FB_COMMAND_PORT             0x3D4
#define FB_DATA_PORT                0x3D5
// commonds define for FB port
#define FB_HIGH_BYTE_COMMAND        14
#define FB_LOW_BYTE_COMMAND         15

extern char read_port(uint16 port);
extern void write_port(uint16 port, uint8 data);
extern void write_port16(uint16 port, uint16 data);
extern void write_port32(uint16 port, uint32 data);
//extern void outb(uint16 port, uint16 value);

/*
Primary PIC (PIC1):
IRQ0: System Timer - IDT Vector 0x20
IRQ1: Keyboard Controller - IDT Vector 0x21
IRQ2: Cascade to Secondary PIC - Not used as a separate vector
IRQ3: COM2 (Serial Port) - IDT Vector 0x23
IRQ4: COM1 (Serial Port) - IDT Vector 0x24
IRQ5: LPT2 (Parallel Port) - IDT Vector 0x25
IRQ6: Floppy Disk Controller - IDT Vector 0x26
IRQ7: LPT1 (Parallel Port) - IDT Vector 0x27

Secondary PIC (PIC2):
IRQ8: Real-Time Clock (RTC) - IDT Vector 0x28
IRQ9: ACPI (Advanced Configuration and Power Interface) - IDT Vector 0x29
IRQ10: Open for use or can be connected to the IRQ2 of the primary PIC - Not typically used
IRQ11: Open for use or can be connected to the IRQ2 of the primary PIC - Not typically used
IRQ12: PS/2 Mouse Controller - IDT Vector 0x2C
IRQ13: Math Coprocessor (FPU) - IDT Vector 0x2D
IRQ14: Primary IDE Channel - IDT Vector 0x2E
IRQ15: Secondary IDE Channel - IDT Vector 0x2F
*/
#endif