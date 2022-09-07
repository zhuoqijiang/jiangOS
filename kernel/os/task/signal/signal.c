#include "signal.h"
#include "../scheduler/scheduler.h"
#include "../../../base/stdio.h"
//static void signal_kill(void* arg);
//static void signal_child(void* arg);
static void signal_kill(void* arg)
{
	task_t* task = get_cur_task();
	kernel_delete_task(task->pid);
}

static void signal_child(void* arg)
{

}

static void send_signal_kill(signal_t* signal)
{
	int task_id = *((int*)signal->arg);

	task_t* task = get_task_by_id(task_id);
	
	if (task) {
		queue_push(&task->signals, signal);
	}
}

static void send_signal_child(void* arg)
{
	
}

static void handle_signal(signal_t* signal)
{
	switch (signal->type) {
	case SIGNAL_KILL:
		signal_kill(signal);
		break;
	case SIGNAL_CHILD:
		signal_child(signal);
		break;
	}
}

void handle_signals(task_t* task)
{
	while (queue_length(&task->signals)) {
		signal_t signal;
		queue_pop(&task->signals, &signal);
		handle_signal(&signal);
	}
}



void send_signal(signal_t* signal)
{
	switch (signal->type) {
	case SIGNAL_KILL:
		send_signal_kill(signal);
		break;
	case SIGNAL_CHILD:
		signal_child(signal);
		break;
	}
}


