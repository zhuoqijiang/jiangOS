#pragma once


#include "../../base/type.h"
#include "../../base/const.h"
#include "../file/file_cache.h"
#include "../task/signal/signal.h"
void sys_call();


int open(const char* pathname, int flags);
size_t write(int fd, const void* buf, size_t count);
size_t read(int fd, void* buf, size_t count);
int close(int fd);


void delete_file(int fd);
int file_list(dir_entry_t* (*entrys)[MAX_INODE_QUANTITY]);

void kill(signal_t* signal);

int get_task_id();
