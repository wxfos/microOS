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
extern void outb(uint16 port, uint16 value);


#endif