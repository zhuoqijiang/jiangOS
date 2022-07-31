#include "../list.h"
#include <stdio.h>
#include <malloc.h>
#include <assert.h>
int test_example[500][500] = {
	{0},
	{},
	{1,2,3,4},
	{1,2,3,8,4},
	{1,2,3,8,100},
	{8,100},
};

void* alloc(size_t type_size) {
	return malloc(type_size);
}

void dealloc(void* p) {
	free(p);
}


void test(list_t* list, int a[500], int len) {
	static int cnt = 1;
	assert(list_length(list) == len);

	list_node_t* nodeA = list_begin(list);

	for (int i = 0; i < list_length(list); i++) {
		assert(*((int*)nodeA->data) == a[i]);
		nodeA = nodeA->next;
	}
	printf("%d pass\n", cnt);
	cnt++;
	list_clear(list);
}

void test1(list_t* list)
{
	int a = 0;
	list_node_t* n;
	list_push(list, &a);	
	test(list, test_example[0], 1);
}

void test2(list_t* list)
{
	int a = 0;
	list_node_t* n;
	n = list_push(list, &a);	
	list_delete(list, n);
	
	test(list, test_example[1], 0);
}

void test3(list_t* list)
{	
	int a = 0;
	list_node_t* n;
	a = 1;
	list_push(list, &a);
	a = 2;
	list_push(list, &a);
	a = 3;
	list_push(list, &a);
	a = 4;
	list_push(list, &a);
	test(list, test_example[2], 4);
}
bool compare(const void* a, const void* b) {
	return *(int*)a == *(int*)b;
}
void test4(list_t* list)
{
	int a = 0;
	list_node_t* n;
	a = 1;
	list_push(list, &a);
	a = 2;
	list_push(list, &a);
	a = 3;
	list_push(list, &a);
	a = 4;
	list_push(list, &a);
	n = list_find(list, &a, compare);
	a = 8;
	list_insert(list, n, &a);
	test(list, test_example[3], 5);
}


void test5(list_t* list)
{
	int a = 0;
	list_node_t* n;
	a = 1;
	list_push(list, &a);
	a = 2;
	list_push(list, &a);
	a = 3;
	list_push(list, &a);
	a = 4;
	list_push(list, &a);
	n = list_find(list, &a, compare);
	a = 8;
	list_insert(list, n, &a);
	a = 100;
	list_set_element(list, n, &a);
	test(list, test_example[4], 5);
}

void test6(list_t* list)
{
	int a = 0;
	list_node_t* n;
	a = 1;
	list_push(list, &a);
	a = 2;
	list_push(list, &a);
	a = 3;
	list_push(list, &a);
	a = 4;
	list_push(list, &a);
	n = list_find(list, &a, compare);
	a = 8;
	list_insert(list, n, &a);
	a = 100;
	list_set_element(list, n, &a);
	
	a = 1;
	n = list_find(list, &a, compare); 
	list_delete(list, n);
	a = 2;
	n = list_find(list, &a, compare); 
	list_delete(list, n);
	a = 3;
	n = list_find(list, &a, compare); 
	list_delete(list, n);
	
	test(list, test_example[5], 2);
}

int main()
{
	int a = 0;
	list_t test_list;
	list_init_alloc_dealloc(&test_list, alloc, dealloc);
	list_construct(&test_list, sizeof(int));
	
	test1(&test_list);
	test2(&test_list);
	test3(&test_list);
	test4(&test_list);
	test5(&test_list);
	test6(&test_list);

	list_destory(&test_list);
}
