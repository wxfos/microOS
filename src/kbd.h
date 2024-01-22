#ifndef INCLUDE_KBD_MAP_H
#define INCLUDE_KBD_MAP_H
#include "io.h"

#define IDT_SIZE                    256
#define INTERRUPT_GATE              0x8e
#define KERNEL_CODE_SEGMENT_OFFSET  0x08
#define ENTER_KEY_CODE              0x1c

struct IDT_entry {
    uint16 offset_lowerbits;
    uint16 selector;
    uint8 zero;
    uint8 type_attr;
    uint16 offset_higherbits;
};

extern void kbd_handler(void);
extern void load_idt(uint32 *idt_ptr);

void idt_init(void);
void kbd_init(void);
void kbd_main(void);

#endif