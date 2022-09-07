.PHONY: all 
all : kernel debug_kernel clean

ENTRYPOINT= 0x30400
LD = ld 
ASM = nasm
CC = gcc
LDFLAGS = -m elf_i386 -Ttext=$(ENTRYPOINT) -s -o
DEBUG_LDFLAGS = -m elf_i386 -Ttext=$(ENTRYPOINT) -o
ASMFLAGS = -f elf -o
CCFLAGS = -fno-stack-protector -m32 -c -o 

OBJS = kernel.o start.o string.o io_asm.o io.o interrupt.o keyboard_asm.o scheduler.o task.o protect.o memory_cache_allocator.o memory_page_allocator.o \
	new_scheduler.o o1_scheduler.o list.o queue.o paging.o disk.o file_sys.o file_op.o file_cache.o stdio.o syscall.o syscall_asm.o signal.o
KERNER = kernel 
DEBUG_KERNEL = debug_kernel 

clean:
	rm ./*.o

$(DEBUG_KERNEL): $(OBJS)
	$(LD) $(DEBUG_LDFLAGS) $@ $(OBJS)
	
$(KERNER): $(OBJS)
	$(LD) $(LDFLAGS) $@ $(OBJS)

start.o: start.c
	$(CC) $(CCFLAGS) $@ $<

io_asm.o: base/io.asm
	$(ASM) $(ASMFLAGS) $@ $<

string.o: base/string.c 
	$(CC) $(CCFLAGS) $@ $<

io.o: base/io.c 
	$(CC) $(CCFLAGS) $@ $<

protect.o: os/protect.c 
	$(CC) $(CCFLAGS) $@ $<

interrupt.o: os/interrupt.c 
	$(CC) $(CCFLAGS) $@ $<

task.o: os/task/task.c 
	$(CC) $(CCFLAGS) $@ $<

scheduler.o: os/task/scheduler.c 
	$(CC) $(CCFLAGS) $@ $<

kernel.o: kernel.asm
	$(ASM) $(ASMFLAGS) $@ $<

keyboard_asm.o: os/keyboard.asm
	$(ASM) $(ASMFLAGS) $@ $<

list.o: tools/container/list.c 
	$(CC) $(CCFLAGS) $@ $<
	
queue.o: tools/container/queue.c 
	$(CC) $(CCFLAGS) $@ $<

memory_cache_allocator.o: os/memory/cache_allocator.c 
	$(CC) $(CCFLAGS) $@ $<

memory_page_allocator.o: os/memory/page_allocator.c 
	$(CC) $(CCFLAGS) $@ $<

new_scheduler.o: os/task/scheduler/scheduler.c 
	$(CC) $(CCFLAGS) $@ $<

o1_scheduler.o: os/task/scheduler/o1_scheduler.c 
	$(CC) $(CCFLAGS) $@ $<

paging.o: os/paging.c
	$(CC) $(CCFLAGS) $@ $<
	
disk.o: os/device/disk.c 
	$(CC) $(CCFLAGS) $@ $<

file_sys.o: os/file/file_sys.c  
	$(CC) $(CCFLAGS) $@ $<

file_cache.o: os/file/file_cache.c  
	$(CC) $(CCFLAGS) $@ $<

file_op.o: os/file/file_op.c  
	$(CC) $(CCFLAGS) $@ $<

stdio.o: base/stdio.c 
	$(CC) $(CCFLAGS) $@ $<

syscall.o: os/syscall/syscall.c 
	$(CC) $(CCFLAGS) $@ $<

syscall_asm.o: os/syscall/syscall.asm
	$(ASM) $(ASMFLAGS) $@ $<

signal.o: os/task/signal/signal.c 
	$(CC) $(CCFLAGS) $@ $<
