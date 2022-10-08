#pragma once

#include "kernel/os/task/scheduler/scheduler.h"

#define PRIORITY_MAX_LEVEL 5

typedef struct {
	scheduler_t scheduler;
	queue_t priority_queues[PRIORITY_MAX_LEVEL + 1];	// value list_node
	int level;
}o1_scheduler_t;


void o1_scheduler_init(scheduler_t* sheduler);
task_t* o1_scheduler_schedule(scheduler_t* scheduler);




