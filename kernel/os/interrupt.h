#pragma once
#include "../base/type.h"

typedef struct {
	u16_t offset_low;
	u16_t selector;
	u8_t dcount;
	u8_t attr; //P(1) DPL(2) DT(1) TYPE(4)
	u16_t offset_high;
}gate_t;

//void init_idt();
//void init_8259A();
void register_interrupt_event(const int ev, void* cb, u8_t desc_type, unsigned char privilege);
void open_interrupt();

