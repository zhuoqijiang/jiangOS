
#include "kernel/os/memory/cache_allocator.h"
#include "kernel/os/memory/page_allocator.h" 
memory_cache_allocator_t kernel_cache_allocator;
extern memory_page_allocator_t kernel_page_allocator;

static void* cache_allocator()
{
	return memory_page_allocator_allocate(&kernel_page_allocator);
}
void memory_cache_allocator_init(memory_cache_allocator_t* alloc)
{
	for (int i = 0; i < ALLOCATOR_LIST_SIZE; i++) {
		alloc->free_list[i] = (memory_cache_obj_t*)0;
	}
	alloc->alloc_func = cache_allocator;
}
static int memory_cache_allocator_round_up(int align, int n)
{
	return (n + align - 1) & ~(align - 1);
}

static int memory_cache_allocator_free_list_index(int align, int n)
{
	return n / align - 1;
}


static void memory_cache_allocator_pushobj(memory_cache_allocator_t* alloc, int index, void* p)
{
	memory_cache_obj_t** current_obj = &alloc->free_list[index];
	memory_cache_obj_t* next_obj = alloc->free_list[index];
	*current_obj = (memory_cache_obj_t*)p;
	(*current_obj)->free_list_link = next_obj;

}

static void* memory_cache_allocator_chunk_alloc(memory_cache_allocator_t* alloc)
{
	return alloc->alloc_func();

}
static void* memory_cache_allocator_getobj(memory_cache_allocator_t* alloc, int index)
{
	memory_cache_obj_t* obj = alloc->free_list[index];
	alloc->free_list[index] = obj->free_list_link;
	return obj;

}

static void memory_cache_allocator_rebackobj(memory_cache_allocator_t* alloc, void* ptr, int index) 
{
	memory_cache_obj_t* obj = alloc->free_list[index];
	((memory_cache_obj_t*)ptr)->free_list_link = obj;
	alloc->free_list[index] = (memory_cache_obj_t*)ptr;
}

static void* memory_cache_allocator_refill(memory_cache_allocator_t* alloc, int index)
{
	memory_cache_obj_t* result = (memory_cache_obj_t*)memory_cache_allocator_chunk_alloc(alloc);
	memory_cache_obj_t* current_obj = 0, *next_obj = 0;
	int obj_size = (index + 1) * ALLOCATOR_ALIGN;
	int obj_num = ALLOCATOR_SIZE / obj_size - 1;
	int rest_size = ALLOCATOR_SIZE - (obj_num + 1) * obj_size; 
	current_obj = result + obj_size / sizeof(memory_cache_obj_t);
	alloc->free_list[index] = current_obj;

	while (obj_num--) {
		next_obj = current_obj + obj_size / sizeof(memory_cache_obj_t);
		current_obj->free_list_link = next_obj;
		current_obj = next_obj;
	}
	
	if (rest_size == 0) {
		return result;
	}

	memory_cache_allocator_pushobj(alloc, memory_cache_allocator_free_list_index(ALLOCATOR_ALIGN, rest_size), current_obj);
	return result;
	
}
static void* memory_cache_allocator_getblock(memory_cache_allocator_t* alloc, int size)
{
	int index = memory_cache_allocator_free_list_index(ALLOCATOR_ALIGN, size);
	if (alloc->free_list[index] != 0) {
		return memory_cache_allocator_getobj(alloc, index);
	}
	return memory_cache_allocator_refill(alloc, index);
}
static void memory_cache_allocator_rebackblock(memory_cache_allocator_t* alloc, void* ptr, int size)
{
	int index = memory_cache_allocator_free_list_index(ALLOCATOR_ALIGN, size);
	memory_cache_allocator_rebackobj(alloc, ptr, index);
	
}
void* memory_cache_allocator_allocate(memory_cache_allocator_t* alloc, size_t size)
{
	if (size <= 0 || size >= 4096) {
		return 0;
	}
	
	int real_size = memory_cache_allocator_round_up(ALLOCATOR_ALIGN, size + 4);
	int* p = (int*)memory_cache_allocator_getblock(alloc, real_size);
	*p = real_size;
	p++;
	return p;

	
}
void memory_cache_allocator_deallocate(memory_cache_allocator_t* alloc, void* ptr)
{
	int* p = (int*)ptr;
	p--;
	int real_size = *p;
	memory_cache_allocator_rebackblock(alloc, p, real_size);
}

