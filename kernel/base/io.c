#include "kernel/base/type.h"
#include "kernel/base/io.h"
#include "kernel/base/const.h"
#define screen_width 80
#define screen_height 25

u16_t row = 0;
u16_t column = 0;

void roll_screen()
{

	u16_t* base = (u16_t*)0xb8000;
	for (int i = 1; i < screen_height; i++) {
		for (int j = 0; j < screen_width; j++) {
			base[(i - 1) * screen_width + j] = base[i * screen_width + j]; 
		}
	}
	for (int j = 0; j < screen_width; j++) {
		dis_char(24, j, 0x0f, ' ');
	}
}
void dis_str(char* str)
{
	while (1) {
		if (*str == 0) {
			break;
		}
		if (*str == '\n') {
			dis_char(row, column, 0x0f, ' ');
			column = 0;
			row++;
			str++;
			if (row >= screen_height) {
				roll_screen();
				column = 0;
				row--;
			}
			continue;
		}
		dis_char(row, column, 0x0f, *str);
		column++;
		str++;
		if (column >= screen_width) {
			column = 0;
			row++;
		}
		if (row >= screen_height) {
			roll_screen();
			column = 0;
			row--;
		}
	}
	dis_char(row, column, 0x0f, '_');
}

void dis_char(u16_t r, u16_t c, u8_t color, u8_t ch)
{
	if (r < 0 || c < 0) {
		return ;
	}
	static u16_t* base = (u16_t*)0xb8000;
	u32_t idx = r * screen_width + c;
	u16_t color16 = color;
	color16 <<= 8; 
	base[idx] = (base[idx] & color16) | ch;
}


void to_eoi()
{
	out_byte((u16_t)INT_M_CTL, (u8_t)EOI);
	out_byte((u16_t)INT_S_CTL, (u8_t)EOI);
}

void delay1s()

{

	unsigned char h,i,j,k;
	for(h=5;h>0;h--)
		for(i=4;i>0;i--)
			for(j=116;j>0;j--)
				for(k=214;k>0;k--);

}


void delay05s()

{

	unsigned char h,i,j,k;
	for(h=2;h>0;h--)
		for(i=4;i>0;i--)
			for(j=116;j>0;j--)
				for(k=214;k>0;k--);

}
