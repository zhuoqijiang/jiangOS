#include "stack.h"


void stack_construct(stack_t* stack, size_t type_size) 
{
	list_construct(&stack->list, type_size); 
}

void stack_init_alloc_dealloc(stack_t* stack, void* (*alloc) (size_t), void (*dealloc) (void*)) 
{
	list_init_alloc_dealloc(&stack->list,alloc, dealloc);
}

void stack_push(stack_t* stack, void* v)
{
	list_push(&stack->list, v);
}

void stack_pop(stack_t* stack, void* data)
{
	list_node_t* node = list_end(&stack->list);
	type_copy(node->data, data, stack->list.type_size);
	list_delete(&stack->list, node);
}

size_t stack_length(stack_t* stack)
{
	return list_length(&stack->list);
}

void stack_destory(stack_t* stack)
{
	list_destory(&stack->list);
}


