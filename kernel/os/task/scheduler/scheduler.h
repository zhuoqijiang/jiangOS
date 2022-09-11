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
void scheduler_delete_task(scheduler_t* sche, int task_id);



void kernel_add_task(task_t* task);
void kernel_delete_task(int task_id);
task_t* kernel_schedule();
task_t* get_cur_task();
task_t* get_task_by_id(int task_id);
int get_cur_task_id();
char* get_cur_task_arg();
