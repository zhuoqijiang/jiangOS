#include "kernel/os/task/scheduler/scheduler.h"
#include "kernel/base/stdio.h"
scheduler_t* scheduler = NULL;

void scheduler_add_task(scheduler_t* sche, task_t* task)
{
	list_push(&sche->tasks, task);
}


static task_t* remove_task_by_id(int task_id);
void scheduler_delete_task(scheduler_t* sche, int task_id)
{
	task_t* task = remove_task_by_id(task_id);
	release_task(task);
	
}

void kernel_add_task(task_t* task)
{
	scheduler_add_task(scheduler, task);
}

void kernel_delete_task(int task_id)
{
	scheduler_delete_task(scheduler, task_id);
}
task_t* kernel_schedule()
{
	task_t* cur_task = scheduler->schedule(scheduler);
	//printf("%s ", cur_task->name);
	return cur_task;
}

task_t* get_cur_task()
{
	task_t* cur_task = scheduler->cur_tack;
	return cur_task;
}

static bool task_find_cmp(const void* a, const void* b)
{
	list_node_t* task_nodeA = (list_node_t*)a;
	list_node_t* task_nodeB = (list_node_t*)b;
	return ((task_t*) list_node_data(task_nodeA))->pid == ((task_t*) list_node_data(task_nodeB))->pid;
}

task_t* get_task_by_id(int task_id)
{

	list_node_t* task_node = list_begin(&scheduler->tasks);
	task_t* task = list_node_data(task_node);
	for (int i = 0; i < list_length(&scheduler->tasks); i++) {
		task_node = list_node_next(task_node);
		task = list_node_data(task_node);
		if (task->pid == task_id) {
			return task;
		} 
	}
	return NULL;
}


static task_t* remove_task_by_id(int task_id)
{
	list_node_t* task_node = list_begin(&scheduler->tasks);
	task_t* task = list_node_data(task_node);
	for (int i = 0; i < list_length(&scheduler->tasks); i++) {
		task_node = list_node_next(task_node);
		task = list_node_data(task_node);
		if (task->pid == task_id) {
			list_delete(&scheduler->tasks, task_node);
			return task;
		} 
	}
	return NULL;
}

int get_cur_task_id()
{
	return get_cur_task()->pid;
}

char* get_cur_task_arg()
{
	return get_cur_task()->arg;
}
