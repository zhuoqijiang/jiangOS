#pragma once

#include "type.h"

void type_copy(void* src, void* dst, size_t type_size);

typedef struct list_node_t{
	void* data;
	struct list_node_t* next;
	struct list_node_t* prev;
}list_node_t;

list_node_t* list_node_alloc(void* v,size_t type_size, void* (*alloc) (size_t));

void list_node_dealloc(list_node_t* node, void (*dealloc) (void*));

void list_node_set_element(list_node_t* node, void* v, size_t type_size);

list_node_t* list_node_next(list_node_t* node);

list_node_t* list_node_prev(list_node_t* node);

void* list_node_data(list_node_t* node);

typedef struct {
	list_node_t* head;
	size_t len;
	
	size_t type_size;
    void* (*alloc) (size_t);
    void (*dealloc) (void*);
}list_t;

void list_construct(list_t* list, size_t type_size);

void list_init_alloc_dealloc(list_t* list,void* (*alloc) (size_t), void (*dealloc) (void*));

list_node_t* list_find(list_t* list, void* v, bool (*compare)(const void*, const void*));

list_node_t* list_begin(list_t* list);

list_node_t* list_end(list_t* list);

list_node_t* list_insert(list_t* list, list_node_t* node,void* v);

list_node_t* list_push(list_t* list, void* v);

void list_set_element(list_t* list, list_node_t* node, void* v);

void list_delete(list_t* list, list_node_t* node);

void list_clear(list_t* list);

void list_destory(list_t* list);

size_t list_length(list_t* list);

bool list_empty(list_t* list);




