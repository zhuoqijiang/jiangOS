#pragma once

#include "../../base/type.h" 
#include "../../base/const.h" 

typedef struct {
	char name[MAX_FILE_SYS_NAME];
	u32_t inode_quantity; 
	u32_t page_quantity;

	u32_t data_page_quantity;
	u32_t pmap_page_quantity;
	u32_t inode_array_page_quantity;
	u32_t root_inode_page_quantity;
	//u32_t super_imap_shared_quantity;
	u32_t imap_page_quantity;
	u32_t super_block_page_quantity;

	u32_t data_start_page_num;
	u32_t root_inode_page_num;
	u32_t inode_array_start_page_num;
	u32_t pmap_start_page_num;
	u32_t imap_start_page_num;


}super_block_t;



typedef struct {
	u32_t mode;
	u32_t data_size;
	u32_t start_page;
	u32_t page_quantity;
	
	int cnt; // ref cnt
	int num; // inode num
}inode_t;


typedef struct {
	int inode_num;
	char name[MAX_FILE_NAME];
}dir_entry_t;

void init_fs();
