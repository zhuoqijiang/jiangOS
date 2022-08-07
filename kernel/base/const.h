#pragma once


// gdt
#define SELECTOR_KERNEL_CS 0x08
#define SELECTOR_KERNEL_DS 0X10

#define OLD_GDT_SIZE 3
#define GDT_SIZE 128
#define IDT_SIZE 128

#define DA_C 0x08
#define DA_DRW 0x02

#define DA_TSS 0x89
#define DA_LDT 0x82 

#define PRIVILEGE_TASK 0xf0  // 0xf0 3
#define SYS_TASK 0x90 
// ldt
#define LDT_SIZE 3
#define SELECTOR_LDT_FIRST 0x20 



//tss
#define SELECTOR_TSS_FIRST 0x18


//cs ds....registers

#define SA_RPL_MASK 0xfffc
#define SA_RPL0 0
#define SA_RPL1 1
#define SA_RPL2 2
#define SA_RPL3 3
#define RPL_TASK SA_RPL3 // n

#define SA_TI_MASK 0Xfffb
#define SA_TIG 0
#define SA_TIL 4  //n

//interrupt
#define INT_M_CTL 0X20 
#define INT_M_CTLMASK 0x21
#define INT_S_CTL  0XA0
#define INT_S_CTLMASK 0XA1
#define EOI 0x20 
#define EOI_PORT 0x20
#define KEY_BOARD 0x60

//interrupt vector

#define INT_VECTOR_IRQ0 0x20
#define INT_VECTOR_IRQ8 0X28






// task
#define MAX_TASK_SIZE 120
#define TASK_STACK_SIZE 4096

#define LDT_SEGMENT 18 * 4 







#define SECTOR_SIZE 512
#define PAGE_SIZE 4096
#define KERNEL_MEMORY_START 0x00100000
#define KERNEL_MEMORY_NORMAL_END 0x00700000
#define KERNEL_MEMORY_SIZE  KERNEL_MEMORY_NORMAL_END - KERNEL_MEMORY_START


// disk
#define DISK_REG_DATA  0x1f0                                                                        
#define DISK_REG_FEATURES 0x1f1                                                                     
#define DISK_REG_SECTOR_COUNT 0x1f2                                                                 
#define DISK_REG_LBA_LOW 0x1f3                                                                          
#define DISK_REG_LBA_MID 0x1f4                                                                          
#define DISK_REG_LBA_HIGH 0x1f5                                                                         
#define DISK_REG_DEVICE 0x1f6                                                                       
#define DISK_REG_COMMAND 0X1f7   

#define DISK_DEVICE_LBA_MODE 0xe0
#define DISK_COMMAND_READ 0X20
#define DISK_COMMAND_WRITE 0X30

// file
#define MAX_FILE_NAME 0x20
#define MAX_FILE_SYS_NAME 0x20
#define MAX_INODE_QUANTITY 28
#define MAX_DISK_PAGE_QUANTITY 64
#define MAX_DATA_PAGE_QUANTITY 56
#define MAX_PAGE_QUANTITY 3
#define INODE_SIZE 64
#define MAX_FILE_DESC_SIZE 32
