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

void testA()
{
	while (1) {	
		dis_str("A");
		delay1s();
	}
}
u8_t stack_testA[1024];
Taskinfo taskinfoA;

void testB()
{
	while (1) {
		
		dis_str("B");
		delay1s();
	}
}
u8_t stack_testB[1024];
Taskinfo taskinfoB;

void testC()
{
	while (1) {
		
		dis_str("C");
		delay1s();
	}
}
u8_t stack_testC[1024];
Taskinfo taskinfoC;

void start()
{
	clean_screen();
	reload_gdt();	
	dis_str("gdt init successful\n");
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
	
	char* page_test = memory_page_allocator_allocate(&kernel_page_allocator);
	for (int i = 0; i < PAGE_SIZE; i++) {
		page_test[i] = i % 256; 
	}
	char* page_test1 = memory_page_allocator_allocate(&kernel_page_allocator);
	disk_write(0, &page_test[0]);
	disk_read(0, &page_test1[0]);
	page_test1[0] = 1;
	dis_str(page_test1);
	
	init_taskinfo(&taskinfoA, testA, memory_page_allocator_allocate(&kernel_page_allocator), PAGE_SIZE, "A", 0);
	task_t* taskA = memory_cache_allocator_allocate(&kernel_cache_allocator, sizeof(task_t));
	task_init_by_info(taskA, &taskinfoA);
	scheduler_add_task(scheduler, taskA);
	init_taskinfo(&taskinfoB, testB, memory_page_allocator_allocate(&kernel_page_allocator), PAGE_SIZE, "B", 5);
	task_t* taskB = memory_cache_allocator_allocate(&kernel_cache_allocator, sizeof(task_t));
	task_init_by_info(taskB, &taskinfoB);
	scheduler_add_task(scheduler, taskB);
	return;
}
