
#include "interrupt.h"
#include "../base/const.h"
#include "../base/io.h"
#include "event.h" 
#include "syscall/syscall.h"
u8_t idt_ptr[6];
gate_t idt[IDT_SIZE];
u16_t int_event_table = 0xffff;

const int int_event_base = 0x20; 
const int int_event_keyboard = 0x21;
const int int_event_at = int_event_base + 14; 
static void init_idt()
{
	u16_t* idt_ptr_limit = (u16_t*)&idt_ptr[0];
	u32_t* idt_ptr_base = (u32_t*)&idt_ptr[2];
	*idt_ptr_limit = IDT_SIZE * sizeof(gate_t) - 1;
	*idt_ptr_base = (long long)(&idt[0]);
	set_lidt(idt_ptr);
}

static void init_8259A()
{
	u8_t OCW1_master_mask = int_event_table;
	u8_t OCW1_salve_mask = int_event_table >> 8;
	//Master 8259 ICW1
	out_byte(INT_M_CTL, 0x11);
	//Slave 8259 ICW1
	out_byte(INT_S_CTL, 0x11);
	//Master 8259 ICW2 master entrypoint 0x20
	out_byte(INT_M_CTLMASK, INT_VECTOR_IRQ0);
	//Slave 8259 ICW2  salve entrypoint 0x28
	out_byte(INT_S_CTLMASK, INT_VECTOR_IRQ8);
	//Master 8259 ICW3 IR2 to salve
	out_byte(INT_M_CTLMASK, 0X4);
	//Salve 8259 ICW3  to IR2 master
	out_byte(INT_S_CTLMASK, 0x2);
	//Master ICW4
	out_byte(INT_M_CTLMASK, 0X1);
	//Slave ICW4
	out_byte(INT_S_CTLMASK, 0X1);
	//Master OWC1
	out_byte(INT_M_CTLMASK, OCW1_master_mask);
	//Slave OCW1
	out_byte(INT_S_CTLMASK, OCW1_salve_mask);

	//OCW2

}


void register_interrupt_event(const int ev, void* cb, u8_t desc_type, unsigned char privilege)
{
	gate_t* p_gate = &idt[ev];
	u32_t base = (long long)cb;
	p_gate->offset_low = base & 0xffff;
	p_gate->selector = SELECTOR_KERNEL_CS;
	p_gate->dcount = 0;
	p_gate->attr = desc_type | (privilege << 5);
	p_gate->offset_high = (base >> 16) & 0xffff;
	
	if (ev < 0x20 || ev == 0x50) {
		return;
	}
	int_event_table = int_event_table & (~(1 << (ev - 0x20)));
}

void open_interrupt()
{
	init_idt();
	register_interrupt_event(INT_KEYBOARD_VECTOR, keyboard_handle, INT_GATE_TYPE_DESC, INT_USER_PRIVILEGE);
	register_interrupt_event(INT_CLOCK_VECTOR, clock_handle, INT_GATE_TYPE_DESC, INT_USER_PRIVILEGE);
	register_interrupt_event(INT_SYSCALL_VECTOR, sys_call, INT_GATE_TYPE_DESC, INT_USER_PRIVILEGE);
	//very important, 0x6e, the six stand for user cpl  can use
	//register_interrupt_event(int_event_at, keyboard_handle, 14, 0x4);
	init_8259A();
	//to_msti();
}


