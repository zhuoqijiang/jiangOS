#pragma once


#include "../../base/type.h"
void sys_call();


int open(const char* pathname, int flags);
size_t write(int fd, const void* buf, size_t count);
size_t read(int fd, void* buf, size_t count);
int close(int fd);
