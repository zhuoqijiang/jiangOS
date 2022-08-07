#pragma once
#include "../../base/type.h" 


typedef struct {
	u8_t data;
	u8_t feature;
	u8_t sector_count;
	u8_t lba_low;
	u8_t lba_mid;
	u8_t lba_high;
	u8_t device;
	u8_t command;
}disk_cmd_t;


void disk_read(u32_t page_num, void* dst);

void disk_write(u32_t page_num, void* scr);
