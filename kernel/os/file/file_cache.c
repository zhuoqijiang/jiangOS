#include "kernel/base/stdio.h"
#include "kernel/os/file/file_cache.h" 
#include "kernel/os/memory/page_allocator.h"
#include "kernel/os/device/disk.h"

extern super_block_t super_block;
extern memory_page_allocator_t kernel_page_allocator;
super_block_cache_t super_block_cache;
inode_cache_t inode_cache;
dir_entry_cache_t dir_entry_cache;
data_page_cache_t data_page_cache;
void* data_cache_pages[MAX_PAGE_QUANTITY];


void init_file_struct_cache()
{
	super_block_cache.super_block = &super_block; 
	super_block_cache.page_num = 0;

	inode_cache.inodes = memory_page_allocator_allocate(&kernel_page_allocator);
	inode_cache.imap = memory_page_allocator_allocate(&kernel_page_allocator);
	inode_cache.size = super_block.inode_quantity;
	inode_cache.imap_page_num = super_block.imap_start_page_num;
	inode_cache.inode_page_num = super_block.inode_array_start_page_num;

	dir_entry_cache.dir_entrys = memory_page_allocator_allocate(&kernel_page_allocator);
	dir_entry_cache.size = super_block.inode_quantity;
	dir_entry_cache.page_num = super_block.root_inode_page_num;

	data_page_cache.data_pages = memory_page_allocator_allocate(&kernel_page_allocator);
	data_page_cache.pmap = memory_page_allocator_allocate(&kernel_page_allocator);
	data_page_cache.size = super_block.super_block_page_quantity;
	data_page_cache.pmap_page_num = super_block.pmap_start_page_num;

	for (int i = 0; i < MAX_PAGE_QUANTITY; i++) {
		data_cache_pages[i] = memory_page_allocator_allocate(&kernel_page_allocator);
	}

}


void flush_struct_cache()
{
	disk_write(inode_cache.imap_page_num, inode_cache.imap);
	disk_write(inode_cache.inode_page_num, inode_cache.inodes);
	disk_write(dir_entry_cache.page_num, dir_entry_cache.dir_entrys);
}


void write_data_to_disk(int page_num)
{
	for (int i = 0; i < MAX_PAGE_QUANTITY; i++) {
		disk_write(page_num + i, data_cache_pages[i]);
	}
}


void read_data_from_disk(int page_num)
{
	for (int i = 0; i < MAX_PAGE_QUANTITY; i++) {
		disk_read(page_num + i, data_cache_pages[i]);
	}
}
