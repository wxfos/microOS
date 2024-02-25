GCC_ROOT = /usr/local/bin
CC = $(GCC_ROOT)/x86_64-elf-gcc
LD = $(GCC_ROOT)/x86_64-elf-ld
STRIP = $(GCC_ROOT)/i386-elf-strip
SRC = src/io.c src/kernel.c src/kbd.c src/vga.c
#boot
all: dirs  kernel
	$(LD) -m elf_i386 -T linker.ld -o bin/kernel obj/boot.o obj/io.o obj/kernel.o obj/kbd.o obj/vga.o
	$(STRIP) bin/kernel

dirs:
	@if [ ! -d obj ]; then \
		mkdir obj;\
	fi
	@if [ ! -d bin ]; then \
		mkdir bin;\
	fi

boot: src/boot.asm
	nasm -f elf32 src/boot.asm -o obj/boot.o

kernel: $(SRC)
#	nasm -f elf32 src/io.asm -o obj/io.o
	$(CC) -fno-stack-protector -m32 -c src/kernel.c -o obj/kernel.o
	$(CC) -fno-stack-protector -m32 -c src/kbd.c -o obj/kbd.o
	$(CC) -fno-stack-protector -m32 -c src/vga.c -o obj/vga.o
	$(CC) -fno-stack-protector -m32 -c src/io.c -o obj/io.o

iso: all
	./check_n_pack.sh

run:
	qemu-system-x86_64 -kernel bin/kernel

clean:	
	rm -rf obj bin
