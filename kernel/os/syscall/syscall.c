#include "../../base/const.h"
#include "../file/file_op.h"
void* sys_call_table[MAX_SYSCALL_QUANTITY] = {
	sys_open,
	sys_write,
	sys_read,
	sys_close
};






