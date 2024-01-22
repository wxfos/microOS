#include "vga.h"
#include "kbd.h"

void kernel(void)
{
	init_term();

	print("\t\t\t\tWelcome to microOS!!!\n");
	print("root@kernel# ");

	idt_init();
	kbd_init();

}
