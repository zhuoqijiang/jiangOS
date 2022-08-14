#pragma once 

#include "file_sys.h" 

typedef enum {
	//O_RD,
	O_CTL = 0x1,
	O_RW = 0x2
}FILE_FLAGS;

typedef struct {
	int mode;
	int read_pos;
	int write_pos;
	dir_entry_t* entry;
	bool is_used;
}file_desc_t;

int sys_open(const char* pathname, int flags);
int sys_close(int fd);
size_t sys_write(int fd, const void* buf, size_t count);
size_t sys_read(int fd, void* buf, size_t count);
