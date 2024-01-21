GCC_ROOT = /usr/local/Cellar/i386-elf-gcc/12.2.0_2/bin
UTILS_ROOT = /usr/local/Cellar/i386-elf-binutils/2.37_2/i386-elf/bin
CC = $(GCC_ROOT)/i386-elf-gcc
LD = $(UTILS_ROOT)/ld
STRIP = $(UTILS_ROOT)/strip


all: dirs boot kernel
	$(LD) -m elf_i386 -T linker.ld -o bin/microOS.bin obj/boot.o obj/io.o obj/kernel.o obj/kbd.o obj/vga.o
	$(STRIP) bin/microOS.bin

dirs:
	if [ ! -d obj ]; then \
		mkdir obj;\
	fi
	if [ ! -d bin ]; then \
		mkdir bin;\
	fi

boot: src/boot.asm
	nasm -f elf32 src/boot.asm -o obj/boot.o

kernel: src/io.asm src/kernel.c src/kbd.c src/vga.c
	nasm -f elf32 src/io.asm -o obj/io.o
	$(CC) -fno-stack-protector -m32 -c src/kernel.c -o obj/kernel.o
	$(CC) -fno-stack-protector -m32 -c src/kbd.c -o obj/kbd.o
	$(CC) -fno-stack-protector -m32 -c src/vga.c -o obj/vga.o

iso: all
	./check_n_pack.sh

run:
	qemu-system-x86_64 -kernel bin/microOS.bin

clean:	
	rm -rf obj bin
