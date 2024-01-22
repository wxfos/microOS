#include "vga.h"
static uint16 lfsr = 0xACE1u;
uint16 bit;

static const uint32 VGA_WIDTH = 80;
static const uint32 VGA_HEIGHT = 25;

static uint16 fb_cursor_x = 0, fb_cursor_y = 0;
static uint32 fb_current_loc = 0;

uint32 term_row;
uint32 term_column;
uint8 term_color;
uint16* term_buffer;

static inline uint8 make_color(enum vga_color fg, enum vga_color bg)
{
	return fg | bg << 4;
}

static inline uint16 make_vgaentry(char c, uint8 color)
{
	uint16 c16 = c;
	uint16 color16 = color;
	return c16 | color16 << 8;
}

static inline uint32 strlen(const char* str)
{
	uint32 len = 0;
	
	while(str[len] != '\0')
		len++;

	return len;
}

uint16 s_rand()
{
	bit  = ((lfsr >> 0) ^ (lfsr >> 2) ^ (lfsr >> 3) ^ (lfsr >> 5) ) & 1;
	return lfsr =  (lfsr >> 1) | (bit << 15);
}

uint8 rand(uint8 n)
{
	return (uint8) (s_rand() % n);
}

uint8 random_color()
{
	return make_color(rand(16), rand(16));
}

void init_term(void)
{
	term_row = 0;
	term_column = 0;
	term_color = make_color(COLOR_LIGHT_GREEN, COLOR_DARK_GREY);
	term_buffer = (uint16*) 0xB8000;

	for(uint32 y = 0; y < VGA_HEIGHT; y++)
	{
		for(uint32 x = 0; x < VGA_WIDTH; x++)
		{
			const uint32 index = y * VGA_WIDTH + x;
			term_buffer[index] = make_vgaentry(' ', term_color);
		}
	}
}

void term_setcolor(uint8 color)
{
	term_color = color;
}

void term_putentryat(char c, uint8 color, uint32 x, uint32 y)
{
	const uint32 index = y * VGA_WIDTH + x;
	term_buffer[index] = make_vgaentry(c, color);
  //set_cursor(x + 1, y);
}

void term_putchar(char c)
{
	if(c == '\n')
	{
		term_column = 0;
		term_row++;
    	set_cursor(term_column, term_row);
		return;
	}
	if(c == '\t')
	{
		uint8 less = VGA_WIDTH - term_column;

		if(less >= 4)
		{
			term_column += 4;
		}
		else
		{
			term_column = 4 - less;
			term_row++;
		}
    	set_cursor(term_column, term_row);
    
		return;
	}
	else
	{
		term_putentryat(c, term_color, term_column, term_row);
	}

	if(++term_column == VGA_WIDTH)
	{
		term_column = 0;
		if(++term_row == VGA_HEIGHT)
			term_row = 0;
	}
  	set_cursor(term_column, term_row);
}

void term_writestring(const char* data)
{
	uint32 datalen = strlen(data);

	for(uint32 i = 0; i < datalen; i++)
	{
		term_putchar(data[i]);
	}
}

void print(const char* str)
{
	term_writestring(str);
}

void move_cursor(uint16 pos)
{
    write_port (FB_COMMAND_PORT, FB_HIGH_BYTE_COMMAND);
    write_port (FB_DATA_PORT,    ((pos >> 8) & 0x00FF));
    write_port (FB_COMMAND_PORT, FB_LOW_BYTE_COMMAND);
    write_port (FB_DATA_PORT,    pos & 0x00FF);
}

void set_cursor(uint16 x, uint16 y)
{
    move_cursor(y * VGA_WIDTH + x);
    
    fb_cursor_x = x;
    fb_cursor_y = y;
}
void get_cursor(uint16 *x, uint16 *y)
{    
    *x = fb_cursor_x;
    *y = fb_cursor_y;
}