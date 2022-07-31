#include "scheduler.h" 
#include "../../base/io.h"
#include "../../base/string.h"
extern descriptor_t gdt[GDT_SIZE];

task_t task_table[MAX_TASK_SIZE];
task_t* cur_task = &task_table[0];
u32_t cur_task_id = 0;


int add_task(Taskinfo* info)
{
	for (int i = 0; i < MAX_TASK_SIZE; i++) {
		if (task_table[i].taskstatus == TASK_EMPTY) {
			task_init_by_info(&task_table[i], info);
			/*
			task_table[i].regs.eip = (long long)info->f;
			task_table[i].regs.esp = (long long)info->stack + info->stack_size;
			memcpy(&task_table[i].name, &info->name, 16);
			task_table[i].taskstatus = TASK_WAIT;
			*/
			return 0;
		}
	}
	return -1;
}



void schedule()
{

	u32_t id = cur_task_id;
	
	task_table[id].taskstatus = TASK_WAIT;

	while (1) {
		id++;
		if (id >= MAX_TASK_SIZE) {
			id = 0;
		}
		if (task_table[id].taskstatus == TASK_WAIT) {
			
			cur_task_id = id;
			cur_task = &task_table[id];
			//set_ltr((u8*)&cur_task->tss_segment);
			
			task_run_prepare(cur_task);
			/*
			task_table[id].tss->esp0 = (long long)&task_table[id].regs + sizeof(stack_frame_t);
			set_lldt((u8_t*)&cur_task->ldt_segment);
			task_table[id].taskstatus = TASK_RUN;
			*/
			return;
		}
	}
}

void init_first_process()
{
	/*
	cur_task->tss->esp0 = (long long)&cur_task->regs + sizeof(stack_frame_t);
	set_lldt((u8_t*)&cur_task->ldt_segment);
	cur_task->taskstatus = TASK_RUN;
	*/
	task_run_prepare(cur_task);
}
