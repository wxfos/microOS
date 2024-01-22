#ifndef INCLUDE_VGA_H
#define INCLUDE_VGA_H
#include "io.h"

enum vga_color {
	COLOR_BLACK 		= 0,
	COLOR_BLUE  		= 1,
	COLOR_GREEN 		= 2,
	COLOR_CYAN  		= 3,
	COLOR_RED   		= 4,
	COLOR_MAGENTA 		= 5,
	COLOR_BROWN   		= 6,
	COLOR_LIGHT_GREY  	= 7,
	COLOR_DARK_GREY   	= 8,
	COLOR_LIGHT_BLUE  	= 9,
	COLOR_LIGHT_GREEN 	= 10,
	COLOR_LIGHT_CYAN  	= 11,
	COLOR_LIGHT_RED   	= 12,
	COLOR_LIGHT_MAGENTA = 13,
	COLOR_LIGHT_BROWN   = 14,
	COLOR_WHITE	    	= 15,
};

static inline uint8 make_color(enum vga_color fg, enum vga_color bg);
static inline uint16 make_vgaentry(char c, uint8 color);
static inline uint32 strlen(const char* str);
uint16 s_rand();
uint8 rand(uint8 n);
uint8 random_color();

void init_term(void);
void term_setcolor(uint8 color);
void term_putentryat(char c, uint8 color, uint32 x, uint32 y);
void term_putchar(char c);
void term_writestring(const char* data);
void print(const char* str);
void move_cursor(uint16 pos);
void set_cursor(uint32 x, uint32 y);

#endif