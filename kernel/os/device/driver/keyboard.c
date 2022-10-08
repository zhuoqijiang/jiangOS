#include "kernel/base/stdio.h"
#include "kernel/os/device/driver/keyboard.h"
#include "kernel/os/memory/cache_allocator.h"
#include "kernel/os/task/scheduler/scheduler.h"
keyboard_event_t keyboard_event;

char keymap0[0x80] = {
    0, 0x1b, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '^', 0x7f, 0,
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', 0x0A, 0, 'a', 's',
    'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', ':', 0, 0, 0x5c, 'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, '7', '8', '9', '-', '4', '5', '6', '+', '1',
    '2', '3', '0', '.', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0x5c, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0x5c, 0, 0,
};


void keyboard_event_set_task(int task_id)
{
	keyboard_event.task_id = task_id;
}

char read_key()
{
	if (keyboard_event.task_id != get_cur_task_id()) {
		return 0;
	}
	if (keyboard_event.keys_read_pos == MAX_KEYS) {
		keyboard_event.keys_read_pos = 0;
	}
	if (!keyboard_event.length) {
		return 0;
	}
	keyboard_event.length--;
	return keyboard_event.keys[keyboard_event.keys_read_pos++];
}

static void write_key(char c)
{
	if (keyboard_event.keys_write_pos == MAX_KEYS) {
		keyboard_event.keys_write_pos = 0;
	}
	if (keyboard_event.length >= MAX_KEYS) {
		return; 
	}
	keyboard_event.length++;
	keyboard_event.keys[keyboard_event.keys_write_pos++] = c;
}

void keyboard_event_init()
{
	keyboard_event.task_id = 0;
	keyboard_event.keys_read_pos = 0;
	keyboard_event.keys_write_pos = 0;
	keyboard_event.length = 0;
}
void handle_keyboard(u32_t key)
{
	if (key > 0x79) {
		return ;
	} 
	//printf("%d %d\n", key, keymap0[key]);
	write_key(keymap0[key]);
}
