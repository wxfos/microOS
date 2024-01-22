#include "kbd.h"
#include "vga.h"
#define iret __asm__ ("leave;iret")
void kbd_interrupt();
void timer_interrupt();
void init_pit(uint16 frequency);

static struct IDT_entry IDT[IDT_SIZE];

void idt_set(int idt_no, void* handler){
	uint32 handler_addr = (uint32)handler;
	IDT[idt_no].offset_lowerbits = handler_addr & 0xffff;
	IDT[idt_no].selector = KERNEL_CODE_SEGMENT_OFFSET;
	IDT[idt_no].zero = 0;
	IDT[idt_no].type_attr = INTERRUPT_GATE;
	IDT[idt_no].offset_higherbits = (handler_addr & 0xffff0000) >> 16;

}

void idt_init(void)
{
	uint32 idt_addr;
	uint32 idt_ptr[2];

	// for(int i=0;i<IDT_SIZE;i++)idt_set(i, timer_interrupt);

	idt_set(0x20, timer_interrupt);
	idt_set(0x21, kbd_interrupt);

	/* fill the IDT descriptor */
	idt_addr = (uint32)IDT ;
	idt_ptr[0] = (sizeof (struct IDT_entry) * IDT_SIZE) + ((idt_addr & 0xffff) << 16);
	idt_ptr[1] = idt_addr >> 16 ;

	load_idt(idt_ptr);
	init_pit(1000);

}

char read_port(uint16 port){
	char ret;
	__asm__ __volatile__(
        "mov %1, %%dx;"
        "in %%dx, %0"
        : "=a" (ret)
        : "r" (port)
    );
	// mov		edx, [esp + 4]
	// in		al, dx
	// ret
	return ret;
}
void write_port(uint16 port, uint8 data) {
    __asm__ __volatile__(
        "outb %0, %1"
        :
        : "a"(data), "Nd"(port)
    );
}
void write_port16(uint16 port, uint16 data) {
    __asm__ __volatile__(
        "outw %0, %1"
        :
        : "a"(data), "Nd"(port)
    );
}

void load_idt(uint32 *idt_ptr){
    __asm__ __volatile__(
        "lidt (%0);"
		"sti"
        :
        : "r"(idt_ptr)
    );
}

#define PIC1_DATA_PORT 0x21
#define PIT_COMMAND_PORT 0x43
#define PIT_DATA_PORT 0x40
void init_pit(uint16 frequency) {
    uint16 divisor = 1193180 / frequency;  // 计算频率分频因子

    // write_port(PIT_COMMAND_PORT, 0x36); 	// 0x36表示二进制计数器，发送高字节/低字节，模式3
    // write_port(PIT_DATA_PORT, (uint8)(divisor & 0xFF));    // 低字节
    // write_port(PIT_DATA_PORT, (uint8)(divisor >> 8));      // 高字节

	int i = 1193180000 / frequency;
	write_port(0x43, 0xb6);
	write_port(0x42, i & 0xff);
	write_port(0x42, i >> 8);
	// i = io_in8(0x61);
	// io_out8(0x61, (i | 0x03) & 0x0f);

}
void reverse(char str[], int length) {
    int start = 0;
    int end = length - 1;
    while (start < end) {
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;
        start++;
        end--;
    }
}

char* itoa(char* str, int num, int base) {
    int i = 0;
    int isNegative = 0;

    if (num == 0) {
        str[i++] = '0';
        str[i] = '\0';
        return str;
    }

    if (num < 0 && base == 10) {
        isNegative = 1;
        num = -num;
    }

    while (num != 0) {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }
    if (isNegative) {
        str[i++] = '-';
    }
    str[i] = '\0';
    reverse(str, i);
    return str;
}

int count = 0;
void timer_interrup_handler() {
	count ++;
    if(count % 1000 == 0){
		uint16 x,y;
		char buf[16];
		itoa(buf, count, 10);
		for(int i=0;i<sizeof(buf) && buf[i];i++)
			term_putentryat(buf[i], COLOR_GREEN, i, 0);
	}
}
void timer_interrupt(){
	static char _ch = '0';
	write_port(PIC1_COMMAND_PORT, 0x60); /* 把IRQ-00接收信号结束的信息通知给PIC */

	term_putentryat(_ch, COLOR_LIGHT_GREEN, 0, 0);
	timer_interrup_handler();
	_ch++;
	if(_ch>0x7e)_ch='0';
	iret;
}


