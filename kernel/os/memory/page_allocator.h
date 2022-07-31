#pragma once


#define PAGE_SIZE 4096

typedef struct memory_page_obj {
	struct memory_page_obj* next;
}memory_page_obj_t; 

typedef struct {
	void* start;
	void* end;
	
	memory_page_obj_t* unuse_start;

}memory_page_allocator_t;


void memory_page_allocator_init(memory_page_allocator_t*, void* start, int size);

void* memory_page_allocator_allocate(memory_page_allocator_t* alloc);

void memory_page_allocator_deallocate(memory_page_allocator_t* alloc, void*);

