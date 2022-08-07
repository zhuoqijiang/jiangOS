#pragma once 

#include "file_sys.h" 

typedef enum {
	//O_RD,
	O_CTL = 0x1,
	O_RW = 0x2
}FILE_FLAGS;

typedef struct {
	int mode;
	int pos;
	dir_entry_t* entry;
	bool is_used;
}file_desc_t;

int sys_open(int* fd,const char* pathname, int flags);
int close(int fd);
size_t wrtie(int fd, const void* buf, size_t count);
size_t read(int fd, const void* buf, size_t count);
