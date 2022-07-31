#pragma once

#include "type.h" 

// for showing 
void show(u16_t row, u16_t column, u8_t color, u8_t ch);

void dis_str(char* str);

void dis_char(u16_t row, u16_t column, u8_t color, u8_t ch);
void out_byte(u16_t port, u8_t value);
void out_byte16(u16_t port, u16_t value);

void in_byte(u16_t port, u8_t* value);
void in_byte16(u16_t port, u16_t* value);

void set_lgdt(u8_t* gdt_ptr);

void set_lidt(u8_t* idt_ptr);

void set_lldt(u8_t* ldt_ptr);

void set_ltr(u8_t* tr_ptr);

void to_mcli();

void to_msti();

void to_eoi();

void set_paging();

void delay1s();
void delay05s();
