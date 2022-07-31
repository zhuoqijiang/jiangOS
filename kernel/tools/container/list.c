#include "list.h"


void type_copy(void* src, void* dst, size_t type_size) 
{
	char* c_dst = (char*)dst;
	char* c_src = (char*)src;
	while (type_size--) {
		*c_dst = *c_src;
		c_dst++;
		c_src++;
	}
}

list_node_t* list_node_alloc(void* v, size_t type_size, void* (*alloc) (size_t)) 
{
	void* data = alloc(type_size);
	list_node_t* new_node = (list_node_t*)alloc(sizeof(list_node_t));
	new_node->data = data;
	type_copy(v, new_node->data, type_size);
	new_node->next = NULL;
	new_node->prev = NULL;
	return new_node;
}

void list_node_dealloc(list_node_t* node, void (*dealloc) (void*)) 
{
	dealloc(node->data);
	dealloc(node);
}


void list_node_set_element(list_node_t* node, void* v, size_t type_size) 
{
	type_copy(v, node->data, type_size);
}

list_node_t* list_node_next(list_node_t* node)
{
	return node->next;
}

list_node_t* list_node_prev(list_node_t* node)
{
	return node->prev;
}

void* list_node_data(list_node_t* node)
{
	return node->data;
}

void list_construct(list_t* list, size_t type_size) 
{
	int v = 0;
	list_node_t* new_node = list_node_alloc(&v, type_size, list->alloc);
	list->head = new_node;
	list->head->next = NULL;
	list->len = 0;
	list->type_size = type_size;
}

void list_init_alloc_dealloc(list_t* list,void* (*alloc) (size_t), void (*dealloc) (void*)) 
{
	list->alloc = alloc;
	list->dealloc = dealloc;
}
list_node_t* list_find(list_t* list, void* v, bool (*compare)(const void*, const void*)) 
{
	list_node_t* traverse_node = list->head->next;
	list_node_t* traverse_next_node = NULL;
	for (int i = 0; i < list->len; i++) {
		traverse_next_node = traverse_node->next;
		if (compare(traverse_node->data, v)) {
			return traverse_node;
		}
		traverse_node = traverse_next_node;
	}
	return NULL;
}

list_node_t* list_begin(list_t* list) 
{
	return list->head->next;
}

list_node_t* list_end(list_t* list) 
{
	list_node_t* begin = list_begin(list);
	if (begin == NULL) {
		return NULL;
	}
	return begin->prev;
}

list_node_t* list_insert(list_t* list, list_node_t* node, void* v) 
{
	list_node_t* new_node = list_node_alloc(v, list->type_size, list->alloc);
	if (node == list->head->next) {
		list->head->next = new_node;
	} 

	list_node_t* prev_node = node->prev;

	prev_node->next = new_node;
	node->prev = new_node;
	new_node->next = node;
	new_node->prev = prev_node;
	list->len++;
	return new_node;
}

list_node_t* list_push(list_t* list, void* v) 
{
	list_node_t* new_node = list_node_alloc(v, list->type_size, list->alloc);
	list->len++;
	if (list->head->next == NULL) {
		new_node->next = new_node;
		new_node->prev = new_node;
		list->head->next = new_node;
		return new_node;
	}
	list_node_t* head = list->head->next;
	list_node_t* tail = head->prev;

	head->prev = new_node;
	tail->next = new_node;
	new_node->prev = tail;
	new_node->next = head;
	return new_node;
}

void list_set_element(list_t* list, list_node_t* node, void* v) 
{
	list_node_set_element(node, v, list->type_size);
}


void list_delete(list_t* list, list_node_t* node)
{
	if (list->head->next == node) {
		if (node->next == node) {
			list->head->next = NULL;
		}
		else{
			list->head->next = node->next;
		}
	}
	list_node_t* prev_node = node->prev;
	prev_node->next = node->next;
	prev_node->next->prev = prev_node;
	
	list_node_dealloc(node, list->dealloc);
	list->len--;
}

void list_clear(list_t* list) 
{
	list_node_t* traverse_node = list->head->next;
	list_node_t* traverse_next_node = NULL;
	for (int i = 0; i < list->len; i++) {
		traverse_next_node = traverse_node->next;
		list_node_dealloc(traverse_node, list->dealloc);
		traverse_node = traverse_next_node;
	}
	list->len = 0;
	list->head->next = NULL;
}
void list_destory(list_t* list) 
{
	list_clear(list);
	list_node_dealloc(list->head, list->dealloc);
}


size_t list_length(list_t* list)
{
	return list->len;
}

bool list_empty(list_t* list)
{
	return list->len == 0;
}
