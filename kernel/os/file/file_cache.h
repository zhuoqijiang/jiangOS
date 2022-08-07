#pragma once

#include "file_sys.h"


typedef struct {
	super_block_t* super_block;
	int page_num;
}super_block_cache_t;


typedef struct {
	inode_t* inodes;
	u8_t* imap;
	int size;
	int imap_page_num;
	int inode_page_num;
}inode_cache_t;


typedef struct {
	dir_entry_t* dir_entrys; 
	int size;
	int page_num;
}dir_entry_cache_t;


typedef struct {
	void** data_pages;
	u8_t* pmap;
	int size;
	int pmap_page_num;
}data_page_cache_t;



void init_file_struct_cache();
void flush_struct_cache();
void flush_data_cache(int page_num);
void read_data_to_flush(int page_num);
