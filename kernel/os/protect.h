#pragma once
#include "../base/type.h"


typedef struct {
	u16_t limit_low; 
	u16_t base_low;
	u8_t base_mid;
	u8_t attr1;   //P(1) DPL(2) DT(1) TYPE(4)
	u8_t limit_high_attr2; //G(1) D(1) AVL(1) LIMIT_HIGH(4)
	u8_t base_high; 
}descriptor_t;

typedef struct {
	u32_t backlink;
	u32_t esp0;
	u32_t ss0;
	u32_t esp1;
	u32_t ss1;
	u32_t esp2;
	u32_t ss2;
	u32_t cr3;
	u32_t eip;
	u32_t flags;
	u32_t eax;
	u32_t ecx;
	u32_t edx;
	u32_t ebx;
	u32_t esp;
	u32_t ebp;
	u32_t esi;
	u32_t edi;
	u32_t es;
	u32_t cs;
	u32_t ss;
	u32_t ds;
	u32_t fs;
	u32_t gs;
	u32_t ldt;
	u16_t trap;
	u16_t iobase;
}tss_t;

void reload_gdt();
void open_tss();
void add_descriptor(descriptor_t* desc, u32_t base, u32_t limit, u16_t attribute);
