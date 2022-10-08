#include "kernel/os/file/file_op.h" 
#include "kernel/os/file/file_cache.h"
#include "kernel/os//task/task.h"
#include "kernel/base/string.h"
#include "kernel/base/stdio.h"
#include "kernel/os/task/scheduler/scheduler.h"
extern super_block_cache_t super_block_cache;
extern inode_cache_t inode_cache;
extern dir_entry_cache_t dir_entry_cache;
extern data_page_cache_t data_page_cache;
extern scheduler_t* scheduler;
extern void* data_cache_pages[MAX_PAGE_QUANTITY];


static int file_index(const char* pathname)
{
	int i = 0;
	for (; i < MAX_INODE_QUANTITY; i++) {
		
		if (inode_cache.imap[i] == 0) {
			continue;
		}
		if (strcmp(dir_entry_cache.dir_entrys[i].name, pathname) == 0) {
			return i;
		}
	}
	return -1;
}

static bool file_is_exist(int fd)
{
	task_t* cur_task = get_cur_task(&scheduler);
	file_desc_t* file_desc = &cur_task->file_desc[fd];
	if (strlen(file_desc->entry->name)) {
		return true;
	}
	return false;
}
int sys_open(const char* pathname, int flags)
{
	if (!strlen(pathname)|| !flags) {
		return -1;
	}
	int i = 0;
	if (flags & O_CTL && file_index(pathname) == -1) {
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
		memcpy(dir_entry_cache.dir_entrys[i].name, pathname, strlen(pathname));
		flush_struct_cache();
	}
	
	i = 0;
	if (flags & O_RW) {
		i = file_index(pathname);
		if (i == -1) {
			return -1;
		}
	}

	inode_cache.inodes[i].cnt++;
	//task_t** cur_task = &scheduler->cur_tack;
	task_t* cur_task = get_cur_task(&scheduler);
	int file_desc_index = get_empty_file_desc(cur_task);
	cur_task->file_desc[file_desc_index].entry = &dir_entry_cache.dir_entrys[i];
	cur_task->file_desc[file_desc_index].read_pos = 0;
	cur_task->file_desc[file_desc_index].write_pos = 0;
	cur_task->file_desc[file_desc_index].read = sys_read;
	cur_task->file_desc[file_desc_index].write = sys_write;
	cur_task->file_desc[file_desc_index].mode = flags;
	
	return file_desc_index;
}
size_t sys_write(int fd, const void* buf, size_t count)
{
	if (!file_is_exist(fd)) {
		return -1;
	}
	task_t* cur_task = get_cur_task(&scheduler);
	file_desc_t* file_desc = &cur_task->file_desc[fd];
	if (!(file_desc->mode & O_W)) {
		return -1;
	}
	int page_num = inode_cache.inodes[file_desc->entry->inode_num].start_page; 
	read_data_from_disk(page_num);
	count = file_desc->write_pos + count > MAX_PAGE_QUANTITY * PAGE_SIZE - 1 ? MAX_PAGE_QUANTITY * PAGE_SIZE - 1 - file_desc->write_pos:count; 
	
	memcpy((char*)data_cache_pages[file_desc->write_pos / PAGE_SIZE] + file_desc->write_pos % PAGE_SIZE, buf, count);
	
	//data_pages[file_desc->write_pos / PAGE_SIZE]  
	write_data_to_disk(page_num);
	file_desc->write_pos += count;
	inode_cache.inodes[file_desc->entry->inode_num].data_size += count;
	flush_struct_cache();
	return count;
}

size_t sys_read(int fd, void* buf, size_t count)
{
	if (!file_is_exist(fd)) {
		return -1;
	}
	task_t* cur_task = get_cur_task(&scheduler);
	file_desc_t* file_desc = &cur_task->file_desc[fd];
	if (!(file_desc->mode & O_R)) {
		return -1;
	}
	int page_num = inode_cache.inodes[file_desc->entry->inode_num].start_page; 
	read_data_from_disk(page_num);
	count = file_desc->read_pos + count > MAX_PAGE_QUANTITY * PAGE_SIZE - 1 ? MAX_PAGE_QUANTITY * PAGE_SIZE - 1 - file_desc->read_pos:count; 
	memcpy(buf, (char*)data_cache_pages[file_desc->read_pos / PAGE_SIZE] + file_desc->read_pos % PAGE_SIZE, count);
	file_desc->read_pos += count;
	return count;
}


int sys_close(int fd)
{
	task_t* cur_task = get_cur_task(&scheduler);
	file_desc_t* file_desc = &cur_task->file_desc[fd];
	int inode_num = file_desc->entry->inode_num;
	if(!(--inode_cache.inodes[inode_num].cnt)) {
		sys_delete_file(fd);
	};
	cur_task->file_desc[fd].is_used = false;
	
	return 0;
}

void sys_delete_file(int fd)
{
	task_t* cur_task = get_cur_task(&scheduler);
	file_desc_t* file_desc = &cur_task->file_desc[fd];
	int inode_num = file_desc->entry->inode_num;
	inode_cache.imap[inode_num] = 0 ;
    inode_clear(&inode_cache.inodes[inode_num]);
    dir_entry_clear(&dir_entry_cache.dir_entrys[inode_num]);
	flush_struct_cache();
}

int sys_file_list(dir_entry_t* (*entrys)[MAX_INODE_QUANTITY])
{
	int file_count = 0;
	int i = 0;
	for (; i < MAX_INODE_QUANTITY; i++) {
		if (inode_cache.imap[i] == 1) {
			(*entrys)[file_count] = &dir_entry_cache.dir_entrys[i];
			file_count++;
			continue;
		}
	}
	return file_count;
}

