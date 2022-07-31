#include "disk.h"
#include "../../base/const.h"
#include "../../base/io.h"

static void disk_cmd_read_init(disk_cmd_t* cmd, u32_t sector_num)
{
	u8_t sector_num_low = sector_num & 0x000000ff;
	u8_t sector_num_mid = (sector_num & 0x0000ff00) >> 8;
	u8_t sector_num_high = (sector_num & 0x00ff0000) >> 16;
	u8_t sector_num_sepcial = (sector_num & 0xff000000) >> 24;
	cmd->sector_count = PAGE_SIZE / SECTOR_SIZE;
	cmd->lba_low = sector_num_low;
	cmd->lba_mid = sector_num_mid;
	cmd->lba_high = sector_num_high;
	cmd->device = DISK_DEVICE_LBA_MODE | sector_num_sepcial;
	cmd->command = DISK_COMMAND_READ;
}

static void disk_cmd_write_init(disk_cmd_t* cmd, u32_t sector_num)
{
	u8_t sector_num_low = sector_num & 0x000000ff;
	u8_t sector_num_mid = (sector_num & 0x0000ff00) >> 8;
	u8_t sector_num_high = (sector_num & 0x00ff0000) >> 16;
	u8_t sector_num_sepcial = (sector_num & 0xff000000) >> 24;
	cmd->sector_count = PAGE_SIZE / SECTOR_SIZE;
	cmd->lba_low = sector_num_low;
	cmd->lba_mid = sector_num_mid;
	cmd->lba_high = sector_num_high;
	cmd->device = DISK_DEVICE_LBA_MODE | sector_num_sepcial;
	cmd->command = DISK_COMMAND_WRITE;
}


void disk_read(u32_t sector_num, void* dst)
{
	disk_cmd_t cmd;
	disk_cmd_read_init(&cmd, sector_num);
	
	#ifndef DEBUG
	//out_byte(DISK_REG_FEATURES, cmd.feature);
	out_byte(DISK_REG_SECTOR_COUNT, cmd.sector_count);
	out_byte(DISK_REG_LBA_LOW, cmd.lba_low);
	out_byte(DISK_REG_LBA_MID, cmd.lba_mid);
	out_byte(DISK_REG_LBA_HIGH, cmd.lba_high);
	out_byte(DISK_REG_DEVICE, cmd.device);
	out_byte(DISK_REG_COMMAND, cmd.command);
	u8_t status;
	while (true) {
		in_byte(DISK_REG_COMMAND, &status);
		if ((status & 0x88) == 0x08) {
			break;
		} 
	}
	u16_t* rdst = dst;
	for (int i = 0; i < PAGE_SIZE; i += 2) {
		in_byte16(DISK_REG_DATA, rdst);
		rdst++;
	}
	#endif
}

void disk_write(u32_t sector_num, void* src)
{
	disk_cmd_t cmd;
	disk_cmd_write_init(&cmd, sector_num);
	
	#ifndef DEBUG
	//out_byte(DISK_REG_FEATURES, cmd.feature);
	out_byte(DISK_REG_SECTOR_COUNT, cmd.sector_count);
	out_byte(DISK_REG_LBA_LOW, cmd.lba_low);
	out_byte(DISK_REG_LBA_MID, cmd.lba_mid);
	out_byte(DISK_REG_LBA_HIGH, cmd.lba_high);
	out_byte(DISK_REG_DEVICE, cmd.device);
	out_byte(DISK_REG_COMMAND, cmd.command);
	u8_t status;
	while (true) {
		in_byte(DISK_REG_COMMAND, &status);
		if ((status & 0x88) == 0x08) {
			break;
		} 
	}
	u16_t* rsrc = src ;
	for (int i = 0; i < PAGE_SIZE; i += 2) {
		out_byte16(DISK_REG_DATA, *rsrc);
		rsrc++;
	}
	#endif
}
