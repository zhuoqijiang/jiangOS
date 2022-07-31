#include "protect.h" 
#include "../base/string.h"
#include "../base/const.h"
#include "../base/io.h"

u8_t gdt_ptr[6];
descriptor_t gdt[GDT_SIZE];
tss_t tss;

void reload_gdt()
{
	void* oldGdt = (void*)(0x7e00);
	void* newGdt = (void*)(&gdt[0]);
	memcpy(newGdt, oldGdt, OLD_GDT_SIZE * sizeof(descriptor_t));
	u16_t* gdt_ptr_limit = (u16_t*)&gdt_ptr[0];
	u32_t* gdt_ptr_base = (u32_t*)&gdt_ptr[2];
	*gdt_ptr_limit = GDT_SIZE * sizeof(descriptor_t) - 1;
	*gdt_ptr_base = (long long)(&gdt[0]);
	set_lgdt(gdt_ptr);
}

void add_descriptor(descriptor_t* desc, u32_t base, u32_t limit, u16_t attribute)
{
	desc->limit_low = limit & 0x0ffff;
	desc->base_low = base & 0x0ffff;
	desc->base_mid = (base >> 16) & 0x0ff;
	desc->attr1 = attribute & 0xff;
	desc->limit_high_attr2 = ((limit >> 16) & 0x0f) | (attribute >> 8)& 0xf0;
	desc->base_high = (base >> 24) & 0x0ff;
}


void open_tss()
{
	u32_t tss_sel = SELECTOR_TSS_FIRST;
	tss.iobase = sizeof(tss_t);
	tss.ss0 = SELECTOR_KERNEL_DS;
	add_descriptor(&gdt[SELECTOR_TSS_FIRST >> 3], (long long)&tss,  sizeof(tss_t) - 1, DA_TSS);
	set_ltr((u8_t*)&tss_sel);
}
