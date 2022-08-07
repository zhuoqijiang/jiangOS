#include "page_allocator.h"

memory_page_allocator_t kernel_page_allocator;


void memory_page_allocator_init(memory_page_allocator_t* alloc, void* start, int size) {
	if (size < 0) {
		return;
	}
	alloc->start = start;
	alloc->end = (void*)((char*)alloc->end + size);
	
	alloc->unuse_start = (memory_page_obj_t*)start;
	memory_page_obj_t* unuse_ptr = alloc->unuse_start;
	for (int i = 0; i < size / PAGE_SIZE - 1; i++) {
		unuse_ptr->next = unuse_ptr + PAGE_SIZE / sizeof(memory_page_obj_t);
		unuse_ptr = unuse_ptr->next;
	}
	unuse_ptr->next = 0;
	return;
}

void* memory_page_allocator_allocate(memory_page_allocator_t* alloc) {
	void* next_unuse_ptr = alloc->unuse_start->next;
	char* page_ptr = (char*)alloc->unuse_start;
	alloc->unuse_start = next_unuse_ptr;
	
	for (int i = 0; i < PAGE_SIZE; i++) {
		page_ptr[i] = 0;
	}
	return page_ptr;
}

void memory_page_allocator_deallocate(memory_page_allocator_t* alloc, void* page_ptr) {
	void* unuse_ptr = alloc->unuse_start;
	((memory_page_obj_t*)page_ptr)->next = unuse_ptr;
	alloc->unuse_start = page_ptr;
}

