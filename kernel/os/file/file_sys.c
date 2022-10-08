#include "kernel/os/file/file_sys.h" 
#include "kernel/base/string.h"
#include "kernel/os/memory/page_allocator.h"
#include "kernel/os/device/disk.h" 
extern memory_page_allocator_t kernel_page_allocator;
super_block_t super_block;
void init_fs()
{
	char* flush = memory_page_allocator_allocate(&kernel_page_allocator);
	char file_sys_name[MAX_FILE_SYS_NAME] = "jiangOS";
	memcpy(super_block.name, file_sys_name, MAX_FILE_SYS_NAME);
	super_block.inode_quantity = MAX_INODE_QUANTITY;
	super_block.page_quantity = MAX_DISK_PAGE_QUANTITY;
	super_block.data_page_quantity =  MAX_DATA_PAGE_QUANTITY;
	super_block.pmap_page_quantity = 1;
	super_block.inode_array_page_quantity = 1;
	super_block.root_inode_page_quantity = 1;
	super_block.imap_page_quantity = 1;
	super_block.super_block_page_quantity = 1;

	super_block.data_start_page_num = super_block.super_block_page_quantity + super_block.imap_page_quantity + super_block.pmap_page_quantity + super_block.inode_array_page_quantity + super_block.root_inode_page_quantity;
	// inode map  shared with super block is one
	
	super_block.root_inode_page_num = super_block.super_block_page_quantity + super_block.imap_page_quantity + super_block.pmap_page_quantity + super_block.inode_array_page_quantity;
	
	super_block.inode_array_start_page_num = super_block.super_block_page_quantity + super_block.imap_page_quantity + super_block.pmap_page_quantity;

	super_block.pmap_start_page_num = super_block.super_block_page_quantity + super_block.imap_page_quantity;
	
	super_block.imap_start_page_num = super_block.super_block_page_quantity;

	memset(flush, 0, PAGE_SIZE);
	memcpy(flush, &super_block, sizeof(super_block_t));
	disk_write(0, flush);
	
	memset(flush, 0, PAGE_SIZE);
	
	for (int i = 1; i < MAX_DISK_PAGE_QUANTITY; i++) {
		disk_write(i, flush);
	}

	memory_page_allocator_deallocate(&kernel_page_allocator, flush);

}

void dir_entry_clear(dir_entry_t * entry)
{
	memset(entry, 0, sizeof(dir_entry_t));
}
void inode_clear(inode_t* inode)
{
	memset(inode, 0, sizeof(inode_t));
}
