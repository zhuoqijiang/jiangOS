#include"../page_allocator.h" 
#include "stdlib.h"
#include "stdio.h"



int main() {
	
	void* test = malloc(4096 * 16);	
	memory_page_allocator_t alloc;

	memory_page_allocator_init(&alloc, test, 4096 * 16);
	
	void *p = memory_page_allocator_allocate(&alloc);
	printf("%p\n", p);
	void *p1 = memory_page_allocator_allocate(&alloc);
	printf("%p\n", p);
	
	memory_page_allocator_deallocate(&alloc, p1);
	memory_page_allocator_deallocate(&alloc, p);
	void *now = memory_page_allocator_allocate(&alloc);
	for (int i = 0; i < 15; i++) {
		void *last = now;
		now = memory_page_allocator_allocate(&alloc);
		printf("%p %ld\n", last, (char*)now - (char*)last);
		
	}
}
