
#include "stdlib.h"
#include "stdio.h"
#include "../cache_allocator.h"


void* test()
{
	return malloc(4096);
}
int main()
{
	memory_cache_allocator_t alloc;
	memory_cache_allocator_init(&alloc, test);
	int n = 0;
	for (int i = 0; i < 20; i++) {
		void* p = memory_cache_allocator_allocate(&alloc, 22);
		n++;
		printf("%d %p %d\n", *((int*)(p - 4)), (int*)(p - 4), n);
		memory_cache_allocator_deallocate(&alloc, p);
	}

	for (int i = 0; i < 20; i++) {
		void* p = memory_cache_allocator_allocate(&alloc, 88);
		n++;
		printf("%d %p %d\n", *((int*)(p - 4)), (int*)(p - 4), n);
	}
	
}
