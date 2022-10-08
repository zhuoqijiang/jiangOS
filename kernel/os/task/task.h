#pragma once
#include "kernel/base/type.h"
#include "kernel/base/const.h"
#include "kernel/os/protect.h"
#include "kernel/os/file/file_op.h"
#include "kernel/tools/container/queue.h"
typedef void* taskinfo_f;


typedef struct {
	u32_t gs;
	u32_t fs;
	u32_t es;
	u32_t ds;
	u32_t edi;
	u32_t esi;
	u32_t ebp;
	u32_t kernel_esp;
	u32_t ebx;
	u32_t edx;
	u32_t ecx;
	u32_t eax;
	u32_t retaddr;
	u32_t eip;
	u32_t cs;
	u32_t eflags;
	u32_t esp;
	u32_t ss;
}stack_frame_t;

typedef struct {
	taskinfo_f f;
	u8_t* stack;
	u32_t stack_size;
	char name[16];
	int priority;
	char arg[64];
}Taskinfo;

typedef enum {
	TASK_EMPTY,
	TASK_RUN,
	TASK_SLEEP,
	TASK_WAIT
}Taskstatus;

typedef struct {
	stack_frame_t regs;
	u16_t ldt_segment;
	descriptor_t ldts[LDT_SIZE];
	tss_t* tss;
	Taskstatus taskstatus;
	char name[16];
	u32_t pid;
	int o1_priority;
	file_desc_t file_desc[MAX_FILE_DESC_SIZE];
	queue_t signals;
	char arg[64];
}task_t;


void task_init(task_t* task, u32_t offset);
task_t* get_task_by_info(Taskinfo* taskinfo);
void task_init_by_info(task_t* task, Taskinfo* taskinfo);
void init_taskinfo(Taskinfo* info, taskinfo_f f, u8_t* s, u32_t ss, char* name, int priority, void* arg);

void task_run_prepare(task_t* task);
void task_wait_prepare(task_t* task);

int get_empty_file_desc(task_t* task);
void release_task(task_t* task);
