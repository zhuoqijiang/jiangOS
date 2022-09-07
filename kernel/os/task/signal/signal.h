#pragma once

#include "../task.h"

typedef enum {
	SIGNAL_KILL,
	SIGNAL_CHILD
}SIGNAL_TYPE;

typedef struct {
	SIGNAL_TYPE type;
	void* arg;
}signal_t;


void handle_signals(task_t* task);

void send_signal(signal_t* signal);
