#pragma once

#include "kernel/base/type.h"

#define ALLOCATOR_LIST_SIZE 128
#define ALLOCATOR_ALIGN 32
#define ALLOCATOR_SIZE 4096


typedef union memory_cache_obj_t {
		union memory_cache_obj_t* free_list_link;
		char client_data[1];
}memory_cache_obj_t;

typedef struct {
	
	memory_cache_obj_t* free_list[ALLOCATOR_LIST_SIZE];

	void* (*alloc_func)();
}memory_cache_allocator_t;


void memory_cache_allocator_init(memory_cache_allocator_t* alloc);
void* memory_cache_allocator_allocate(memory_cache_allocator_t* alloc, size_t size);
void memory_cache_allocator_deallocate(memory_cache_allocator_t* alloc, void* p);





