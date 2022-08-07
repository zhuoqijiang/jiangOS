#include "file_op.h" 

#include "../task/task.h"
#include "file_cache.h"
#include "../../base/string.h"
#include "../task/scheduler/scheduler.h"
extern super_block_cache_t super_block_cache;
extern inode_cache_t inode_cache;
extern dir_entry_cache_t dir_entry_cache;
extern data_page_cache_t data_page_cache;
extern scheduler_t* scheduler;

int open(const char* pathname, int flags)
{
	int i = 0;
	if (flags & O_CTL) {
		for (; i < MAX_INODE_QUANTITY; i++) {
			if (inode_cache.imap[i] == 0) {
				inode_cache.imap[i] = 1;
				break;
			}
		}
		inode_cache.inodes[i].num = i;
		inode_cache.inodes[i].page_quantity = MAX_PAGE_QUANTITY;
		inode_cache.inodes[i].start_page = super_block_cache.super_block->data_start_page_num  + MAX_PAGE_QUANTITY * i;
		dir_entry_cache.dir_entrys[i].inode_num = i;
		memcpy(dir_entry_cache.dir_entrys[i].name, pathname, MAX_FILE_NAME);
		flush_struct_cache();
	}
	
	i = 0;
	if (flags & O_RW) {
		for (; i < MAX_INODE_QUANTITY; i++) {
			if (strcmp(dir_entry_cache.dir_entrys[i].name, pathname) == 0) {
				break;
			}
		}
	}
	task_t** cur_task = &scheduler->cur_tack;
	
	int file_desc_index = get_empty_file_desc(*cur_task);
	(*cur_task)->file_desc[file_desc_index].entry = &dir_entry_cache.dir_entrys[i];
	(*cur_task)->file_desc[file_desc_index].pos = 0;
	return file_desc_index;
}
/*
size_t wrtie(int fd, const void* buf, size_t count)
{

}
size_t read(int fd, const void* buf, size_t count)
{

}

int close(int fd)
{

}

*/
