
#include "task.h"
#include "../../base/string.h"
#include "../../base/io.h"
extern descriptor_t gdt[GDT_SIZE];
extern tss_t tss;


u8_t pid_upon[MAX_TASK_SIZE];
u8_t kernel_stack[TASK_STACK_SIZE];
void task_init(task_t* task, u32_t offset)
{
		task->ldt_segment = SELECTOR_LDT_FIRST + (offset << 3);
		memcpy(&task->ldts[0], &gdt[SELECTOR_KERNEL_CS >> 3], sizeof(descriptor_t));
		task->ldts[0].attr1 = DA_C | (PRIVILEGE_TASK);
		memcpy(&task->ldts[1], &gdt[SELECTOR_KERNEL_DS >> 3], sizeof(descriptor_t));
		task->ldts[1].attr1 = DA_DRW | (PRIVILEGE_TASK);
		task->regs.cs = (0 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
		task->regs.ds = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
		task->regs.es = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
		task->regs.fs = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
		task->regs.ss = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;
		task->regs.gs = (8 & SA_RPL_MASK & SA_TI_MASK) | SA_TIL | RPL_TASK;

		//task->regs.eip = (long long)test_task;
		//task->regs.esp = (long long) task_stack + TASK_STACK_SIZE;
		task->regs.eflags = 0x1202; // IF = 0,IOPL = 1,bit 2 is always 1
	// if need close, close int
		add_descriptor(&gdt[task->ldt_segment >> 3], (long long)task->ldts , LDT_SIZE * sizeof(descriptor_t) - 1, DA_LDT);
		//set_lldt((u8_t*)&task->ldt_segment);	
		//task->tss.iobase = sizeof(Tss);
		//task->tss.ss0 = SELECTOR_KERNEL_DS;
		//task->tss.esp0 = (long long)&task->regs + sizeof(StackFrame);
		//if (offset == 0) {
		//	init_descriptor(&gdt[task->tss_segment >> 3], (long long)&task->tss,  sizeof(Tss) - 1, DA_TSS);
		//	set_ltr((u8_t*)&task->tss_segment);
		//}
		task->tss = &tss;
		task->taskstatus = TASK_EMPTY;
		task->pid = offset;
}

static int task_get_pid()
{
	for (int i = 0; i < MAX_TASK_SIZE; i++) {
		if (pid_upon[i] == 0) {
			pid_upon[i] = 1;
			return i;
		}
	}
	return -1;
}

void task_init_by_info(task_t* task, Taskinfo* taskinfo) 
{
	int pid = task_get_pid();
	task_init(task, pid);
	task->regs.eip = (long long)taskinfo->f;
	task->regs.esp = (long long)taskinfo->stack + taskinfo->stack_size;
	memcpy(&task->name, &taskinfo->name, 16);
	task->taskstatus = TASK_WAIT;
	task->o1_priority = taskinfo->priority;
	for (int i = 0; i < MAX_FILE_DESC_SIZE; i++) {
		task->file_desc[i].is_used = 0;	
	}
}

void task_run_prepare(task_t* task) 
{
	task->tss->esp0 = (long long)&task->regs + sizeof(stack_frame_t);
	set_lldt((u8_t*)&task->ldt_segment);
	task->taskstatus = TASK_RUN;
}

void task_wait_prepare(task_t* task)
{
	task->taskstatus = TASK_WAIT;
}
void init_taskinfo(Taskinfo* info, taskinfo_f f, u8_t* s, u32_t ss, char* name, int priority)
{
	info->f = f;
	info->stack = s;
	info->stack_size = ss;
	info->priority = priority;
	memcpy(&info->name, name, 16);
}


int get_empty_file_desc(task_t* task)
{
	for (int i = 0; i < MAX_FILE_DESC_SIZE; i++) {
		if (task->file_desc[i].is_used == 0) {
			task->file_desc[i].is_used = 1;
			return i;
		}
	}
	return -1;
}
