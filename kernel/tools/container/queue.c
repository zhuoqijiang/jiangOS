#include "kernel/tools/container/queue.h"


void queue_construct(queue_t* queue, size_t type_size) 
{
	list_construct(&queue->list, type_size); 
}

void queue_init_alloc_dealloc(queue_t* queue, void* (*alloc) (size_t), void (*dealloc) (void*)) 
{
	list_init_alloc_dealloc(&queue->list,alloc, dealloc);
}

void queue_push(queue_t* queue, void* v)
{
	list_push(&queue->list, v);
}

void queue_pop(queue_t* queue, void* data)
{
	list_node_t* node = list_begin(&queue->list);
	type_copy(node->data, data, queue->list.type_size);
	list_delete(&queue->list, node);
}

size_t queue_length(queue_t* queue)
{
	return list_length(&queue->list);
}

void queue_destory(queue_t* queue)
{
	list_destory(&queue->list);
}

bool queue_empty(queue_t* queue)
{
	return list_empty(&queue->list);
}
