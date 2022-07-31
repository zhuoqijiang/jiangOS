#include "scheduler.h"

scheduler_t* scheduler = NULL;

void scheduler_add_task(scheduler_t* sche, task_t* task)
{
	list_push(&sche->tasks, task);
}

void scheduler_delete_task(scheduler_t* sche, u32_t task_id)
{
	//:list_find()
}


void kernel_add_task(task_t* task)
{
	scheduler_add_task(scheduler, task);
}
void kernel_delete_task(u32_t pid)
{
	scheduler_delete_task(scheduler, pid);
}
task_t* kernel_schedule()
{
	task_t * cur_task = scheduler->schedule(scheduler);
	return cur_task;
}
