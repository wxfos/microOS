#include "vga.h"
#include "kbd.h"
//section .text
char stack_space[16384];
uint32 data[]={0x1BADB002, 0, - 0x1BADB002};

void kernel(void)
{
	idt_init();
	kbd_init();

	init_term();

	print("\t\t\t\tWelcome to microOS!!!\n");
	print("root@kernel# ");

}
void start(){
	uint32 p = (uint32)stack_space;
	p += sizeof(stack_space);
	__asm__ (
        "mov %0, %%esp"::"r"(p)
	);
	kernel();
	while(1)__asm__ ("hlt");

}
