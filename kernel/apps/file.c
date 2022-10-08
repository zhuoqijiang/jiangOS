#include "kernel/os/syscall/syscall.h"
#include "kernel/os/file/file_op.h"
#include "kernel/os/file/file_cache.h" 
#include "kernel/os/file/file_sys.h"
#include "kernel/base/stdio.h"
#include "kernel/base/string.h"
#include "kernel/os/device/driver/keyboard.h"
void ls()
{
	dir_entry_t* entrys[MAX_INODE_QUANTITY];
	int fileCount = file_list(&entrys);
	for (int i = 0; i < fileCount; i++) {
		printf("%s ", entrys[i]->name);
	}
	printf("\n");
}


void touch(const char* name)
{
	open(name, O_CTL);
}


void rm(const char* name)
{
	int fd = open(name, O_RW);
	delete_file(fd);
	close(fd);
}


void cat(const char* name)
{
	int fd = open(name, O_RW);
	if (fd < 0) {
		printf("file no found\n");
		return; 
	}
	
	const int max_buf_size = 256;
	char buf[max_buf_size];
	memset(buf, 0, max_buf_size);
	while (read(fd, &buf, max_buf_size) > 0) {
		printf("%s", buf);
		memset(buf, 0, max_buf_size);
	}
	printf("\n");
}

void edit(const char* name)
{
	int fd = open(name, O_RW);
	if (fd < 0) {
		printf("file no found\n");
		return; 
	}

	char c;
	const int max_buf_size = 256;
	char buf[max_buf_size];
	memset(buf, 0, max_buf_size);
	int now_write = 0;
	while (true) {
		c = read_key();
		if (!c) {
			continue;
		}
		if (c == 0x1b) {
			write(fd, buf, strlen(buf));
			break;
		}
		printf("%c", c);
		if (now_write >= max_buf_size) {
			write(fd, buf, strlen(buf));
			memset(buf, 0, max_buf_size);
			now_write = 0;
		}
		buf[now_write++] = c;
		
	}
	printf("\n");
}
