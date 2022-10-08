#pragma once 

#include "kernel/os/file/file_sys.h" 

typedef enum {
	//O_RD,
	O_CTL = 0x1,
	O_R = 0x2, 
	O_W = 0x4,
	O_RW = 0x6
}FILE_FLAGS;

typedef enum {
	O_INPUT,
	O_OUTPUT,
	O_FILE
}MODE_FLAGS;

typedef struct {
	int mode;
	int read_pos;
	int write_pos;
	dir_entry_t* entry;

	size_t (*read)(int fd, void* buf, size_t count);
	size_t (*write)(int fd, const void* buf, size_t count);

	bool is_used;
}file_desc_t;

void file_desc_init(file_desc_t* fd, int mode);
int sys_open(const char* pathname, int flags);
int sys_close(int fd);
size_t sys_write(int fd, const void* buf, size_t count);
size_t sys_read(int fd, void* buf, size_t count);
void sys_delete_file(int fd);
int sys_file_list(dir_entry_t* (*entrys)[MAX_INODE_QUANTITY]);
