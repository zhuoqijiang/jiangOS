#pragma once


typedef enum {
	SIGNAL_KILL,
	SIGNAL_CHILD
}SIGNAL_TYPE;

typedef struct {
	int type;
	void* arg;
}signal_t;

void handle_signal();
