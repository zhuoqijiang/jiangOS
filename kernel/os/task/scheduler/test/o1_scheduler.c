#include "../o1_scheduler.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define TASK_NUM 5

task_t tasks[TASK_NUM];

void add_tasks(scheduler_t* sche)
{
	for (int i = 0; i < TASK_NUM; i++) {
		tasks[i].pid = i;
		tasks[i].o1_priority = i;
		scheduler_add_task(sche, &tasks[i]);
	}
}


int main()
{
	scheduler_t* sche = malloc(sizeof(o1_scheduler_t));
	o1_scheduler_init(sche);
	add_tasks(sche);
	
	while (1) {
		sche->schedule(sche);
		printf("%d\n",sche->cur_tack->pid); 
		sleep(1);
	}

}
//gcc o1_scheduler.c ../o1_scheduler.c ../scheduler.c ../../../../tools/container/list.c ../../../../tools/container/queue.c -o o1_scheduler
