#include "kernel/os/task/scheduler/o1_scheduler.h"

#include "kernel/os/memory/cache_allocator.h"
#include "kernel/os/task/signal/signal.h" 
 
extern memory_cache_allocator_t kernel_cache_allocator;

static void* scheduler_allocate(size_t size)
{
	return memory_cache_allocator_allocate(&kernel_cache_allocator, size);
}
static void scheduler_deallocate(void* p)
{
	memory_cache_allocator_deallocate(&kernel_cache_allocator, p);
}
void o1_scheduler_init(scheduler_t* scheduler)
{
	o1_scheduler_t* sche = scheduler;
	sche->scheduler.schedule = o1_scheduler_schedule;
	list_init_alloc_dealloc(&sche->scheduler.tasks, scheduler_allocate, scheduler_deallocate);
	list_construct(&sche->scheduler.tasks, sizeof(task_t));
	for (int i = 0; i <= PRIORITY_MAX_LEVEL; i++) { 
		queue_init_alloc_dealloc(&sche->priority_queues[i], scheduler_allocate,  scheduler_deallocate);
		queue_construct(&sche->priority_queues[i], sizeof(list_node_t));
	}
	sche->level = 0;
	sche->scheduler.cur_tack = NULL;
}

static void o1_scheduler_reload(o1_scheduler_t* sche)
{
	list_node_t* task_node = list_begin(&sche->scheduler.tasks);
	task_t* task = list_node_data(task_node);
	for (int i = 0; i < list_length(&sche->scheduler.tasks); i++) {
		queue_push(&sche->priority_queues[task->o1_priority], task_node);	
		task_node = list_node_next(task_node);
		task = list_node_data(task_node);
	}

}
task_t* o1_scheduler_schedule(scheduler_t* scheduler)
{
	o1_scheduler_t* sche = scheduler;
	if (sche->scheduler.cur_tack != NULL) {
		task_wait_prepare(sche->scheduler.cur_tack);
		if (sche->level == PRIORITY_MAX_LEVEL) {
			handle_signals(sche->scheduler.cur_tack);
		}
	}
	while (true) {
		if (sche->level > PRIORITY_MAX_LEVEL) {
			o1_scheduler_reload(sche);
			sche->level = 0;
			continue;
		}
		if (queue_empty(&sche->priority_queues[sche->level])) {
			sche->level++;
			continue;
		}
		list_node_t cur_task_node; 
		queue_pop(&sche->priority_queues[sche->level], &cur_task_node);
		sche->scheduler.cur_tack = cur_task_node.data;
		if (sche->level == PRIORITY_MAX_LEVEL) {
			break;
		}
		queue_push(&sche->priority_queues[sche->level + 1], &cur_task_node);
		break;
	}
	task_run_prepare(sche->scheduler.cur_tack);
	return sche->scheduler.cur_tack;
}



