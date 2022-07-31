#include "../queue.h"
#include <stdio.h>
#include <malloc.h>
#include <assert.h>

void* alloc(size_t type_size) {
	return malloc(type_size);
}

void dealloc(void* p) {
	free(p);
}

int main(){
	int a = 3;
	queue test_queue;
	queue_init_alloc_dealloc(&test_queue, alloc, dealloc);
	queue_construct(&test_queue, sizeof(int));
	
	queue_push(&test_queue, &a);
	a = 4;
	queue_push(&test_queue, &a);
	a = 5;
	queue_push(&test_queue, &a);
	while (queue_length(&test_queue)) {
		int r;
		queue_pop(&test_queue, &r);
		printf("%d ", r);
	}
}

