#pragma once
#include "kernel/base/type.h"
#include "kernel/tools/container/queue.h"
#include "kernel/os/task/task.h"

#define MAX_KEYS 128
typedef struct {
	char keys[MAX_KEYS];
	size_t keys_read_pos;
	size_t keys_write_pos;
	size_t length;
	int task_id;
}keyboard_event_t;



void keyboard_event_set_task(int task_id);
char read_key();
void handle_keyboard(u32_t key);
