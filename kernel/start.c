#include "base/io.h"
#include "os/protect.h"
#include "os/interrupt.h"
#include "os/task/task.h"
#include "os/task/scheduler.h" 
#include "base/string.h"
#include "os/memory/page_allocator.h"
#include "os/memory/cache_allocator.h"
#include "base/const.h"
#include "os/task/scheduler/o1_scheduler.h"
#include "os/paging.h"
#include "os/device/disk.h"
#include "os/file/file_sys.h"
#include "os/file/file_cache.h"
#include "os/file/file_op.h"
#include "base/stdio.h"
#include "os/syscall/syscall.h"
#include "os/device/driver/keyboard.h"


extern memory_page_allocator_t kernel_page_allocator;
extern memory_cache_allocator_t kernel_cache_allocator;
extern scheduler_t* scheduler; 
void clean_screen()
{
	for (u16_t i = 0; i < 80; i++) {
		for (u16_t j = 0; j < 160; j++) {
			show(i, j, 0x0f, ' ');
		}
	}
}

int cnt = 0;
char write_test[PAGE_SIZE];
void testA()
{
	
	int fd,fdA;
	fd = open("test", O_CTL|O_RW);
	fdA = open("testA", O_CTL|O_RW);
	char *test = "testing";
	size_t size = write(fd, test, strlen(test));
	//close(fd);
	
	//printf("%d\n", len);
	/*
	signal_t signal;
	int dst_task_id = 1;
	signal.type = SIGNAL_KILL;
	signal.arg = &dst_task_id;
	kill(&signal);
	*/

	char str[200];
	int index = 0;
	memset(str, 0, 200);
	while (1) {
	//	printf("%d",get_task_id());
		char c = read_key();
		if (c) {
			str[index++] = c;
			if (c == '\n') {
				printf("%s", str);
				memset(str, 0, 200);
				index = 0;
			}
		}
		//delay1s();
	}
}
Taskinfo taskinfoA;

void testB()
{
	/*
	signal_t signal;
	int dst_task_id = 0;
	signal.type = SIGNAL_KILL;
	signal.arg = &dst_task_id;
	kill(&signal);
	*/
	while (1) {
		//printf("B");
		printf("B");
		//printf("%d",get_task_id());
		delay1s();
	}
}
Taskinfo taskinfoB;

char arg[MAX_ARG_SIZE];
extern void ls();
extern void touch(const char* name);
extern void rm(const char* name);
extern void cat(const char* name);
extern void edit(const char* name);

void exec()
{
	char* arg = get_task_arg();
	
	int cnt = 1;
	char cmd[MAX_ARG_SIZE];
	memset(cmd, 0 ,MAX_ARG_SIZE);
	char args[MAX_ARGS][MAX_ARG_SIZE];
	int cmd_end = strfind(arg, ' ', cnt);
	int arg_length = strlen(arg);
	memcpy(cmd, arg, cmd_end);
	
	//printf("%s\n", cmd);
	for (int i = 0; i < MAX_ARGS; i++) {
		cnt++;
		int argx_end = strfind(arg, ' ', cnt - 1);
		int argz_end = strfind(arg, ' ', cnt);
		if (argz_end == -1 || argx_end + 1 >= arg_length) {
			break;
		}
		memcpy(&args[i][0], &arg[argx_end + 1], argz_end - argx_end);
	}
	keyboard_event_set_task(get_task_id());

	
	if (!strcmp(cmd, "ls")) {
		ls();
	}
	else if (!strcmp(cmd, "touch")) {
		touch(args[0]);
	}
	else if (!strcmp(cmd, "rm")) {
		rm(args[0]);
	}
	else if (!strcmp(cmd, "cat")) {
		cat(args[0]);
	}
	else if (!strcmp(cmd, "edit")) {
		edit(args[0]);
	}
	else {
		printf("cmd no found\n");
	}
	
	printf(">>");
	keyboard_event_set_task(0);
	signal_t signal;
	int dst_task_id = get_task_id();
	signal.type = SIGNAL_KILL;
	signal.arg = &dst_task_id;
	kill(&signal);
	while(1){}
}

void start_task()
{
	Taskinfo taskinfo;
	init_taskinfo(&taskinfo, exec, memory_page_allocator_allocate(&kernel_page_allocator), PAGE_SIZE, "ls", 0, arg);
	task_t* task = get_task_by_info(&taskinfo);
	scheduler_add_task(scheduler, task);
}
void super() {
	printf(">>");
	keyboard_event_set_task(get_task_id());
	int index = 0;
	memset(arg, 0, MAX_ARG_SIZE);
	while (1) {
		char c = read_key();
		if (c) {
			
			printf("%c", c);
			if (c == '\n') {
				arg[index] = ' ';
				start_task();
				memset(arg, 0, MAX_ARG_SIZE);
				index = 0;
			}
			else {
				arg[index++] = c;
			}
		}
	}
}

Taskinfo superinfo;
void start()
{
	clean_screen();
	reload_gdt();
	printf("gdt init successful-%s\n", "yes");
	//while(1);
	open_interrupt();
	dis_str("start int service successful\n");
	//init_task_table();
	//dis_str("task table init successful\n");
	open_tss();
	dis_str("tss init successful\n");
	memory_page_allocator_init(&kernel_page_allocator, (void*)KERNEL_MEMORY_START, KERNEL_MEMORY_SIZE);
	memory_cache_allocator_init(&kernel_cache_allocator);
	scheduler = memory_cache_allocator_allocate(&kernel_cache_allocator, sizeof(o1_scheduler_t));
	o1_scheduler_init(scheduler); // init scheduler
	open_paging_model();
	init_fs();
	init_file_struct_cache();

	init_taskinfo(&superinfo, super, memory_page_allocator_allocate(&kernel_page_allocator), PAGE_SIZE, "super", 0, NULL);
	task_t* super = get_task_by_info(&superinfo);
	scheduler_add_task(scheduler, super);
	return;
}
