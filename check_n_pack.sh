#!/bin/sh

if grub-file --is-x86-multiboot kernel; then
	echo Multiboot Confirmed!!!
	mkdir -p isodir/boot/grub
	cp grub.cfg isodir/boot/grub/grub.cfg
	cp bin/kernel isodir/boot/kernel
	grub-mkrescue --arcs-boot -o microOS.iso isodir
else
	echo [ERROR] The file is not multiboot.
fi
