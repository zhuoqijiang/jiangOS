#pragma once

//#include "../task.h"
#include "../../../tools/container/list.h"
#include "../../../tools/container/queue.h" 
#include "../task.h"


typedef struct scheduler_t{
	task_t* cur_tack;
	list_t tasks;
	task_t* (*schedule)(struct scheduler_t*);

}scheduler_t;


void scheduler_add_task(scheduler_t* sche, task_t* task);
void scheduler_delete_task(scheduler_t* sche, u32_t task_id);



void kernel_add_task(task_t* task);
void kernel_delete_task(u32_t pid);
task_t* kernel_schedule();
