#include "../../base/const.h"
#include "../file/file_op.h"
#include "../task/task.h"
#include "../task/scheduler/scheduler.h"
#include "../task/signal/signal.h"
extern scheduler_t* scheduler;
static size_t s_write(int fd, const void* buf, size_t count);
static size_t s_read(int fd, void* buf, size_t count);
void* sys_call_table[MAX_SYSCALL_QUANTITY] = {
	sys_open,
	s_write,
	s_read,
	sys_close,
	sys_delete_file,
	sys_file_list,
	send_signal,
	get_cur_task_id,
	get_cur_task_arg
};


static size_t s_write(int fd, const void* buf, size_t count)
{
	task_t* cur_task = get_cur_task(&scheduler);
	file_desc_t* file_desc = &cur_task->file_desc[fd];
	return file_desc->write(fd, buf, count);
}

static size_t s_read(int fd, void* buf, size_t count)
{
	task_t* cur_task = get_cur_task(&scheduler);
	file_desc_t* file_desc = &cur_task->file_desc[fd];
	return file_desc->read(fd, buf, count);
}



