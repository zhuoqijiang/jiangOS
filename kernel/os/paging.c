#include "kernel/os/paging.h"
#include "kernel/base/const.h"
//#include "stdlib.h" //for test
#include "kernel/os/memory/page_allocator.h"
extern memory_page_allocator_t kernel_page_allocator;
pde_t* kernel_space = NULL;

void pde_set_pte_addr(pde_t *pde, u32_t pageNum)
{
	pde->pde_attribute = (pde->pde_attribute & 0x00000fff) | (pageNum << 12);
}
u32_t pde_get_pte_addr(pde_t* pde)
{
	return pde->pde_attribute >> 12; 
}
void pde_set_p(pde_t* pde, bool flag)
{
	pde->pde_attribute |= flag;
}
void pde_set_rw(pde_t* pde, bool flag)
{
	pde->pde_attribute |= (flag << 1);
}
void pde_set_us(pde_t* pde, bool flag)
{
	pde->pde_attribute |= (flag << 2);
}


void pte_set_page_addr(pte_t* pte, u32_t pageNum)
{
	pte->pte_attribute = (pte->pte_attribute & 0x00000fff) |  (pageNum << 12);
}
u32_t pte_get_page_addr(pte_t* pte)
{
	return pte->pte_attribute >> 12; 
}
void pte_set_p(pte_t* pte, bool flag)
{
	pte->pte_attribute |= flag;
}
void pte_set_rw(pte_t* pte, bool flag)
{
	pte->pte_attribute |= (flag << 1);
}
void pte_set_us(pte_t* pte, bool flag)
{
	pte->pte_attribute |= (flag << 2);
}


static void paging_init_kernel_space()
{
	#ifndef DEBUG
	kernel_space = memory_page_allocator_allocate(&kernel_page_allocator);
	#else 
	kernel_space = malloc(PAGE_SIZE);
	#endif 
	u32_t high = 0; //10
	u32_t mid = 0; //10
	u32_t low = 0; //12
	for (; high <= ((u32_t)KERNEL_MEMORY_NORMAL_END >> 22); high++) {
		#ifndef DEBUG
		pte_t* tableAddr = (pte_t*)memory_page_allocator_allocate(&kernel_page_allocator);
		#else 
		pte_t* tableAddr = malloc(PAGE_SIZE);
		#endif 
		kernel_space[high].pde_attribute = 0;
		pde_set_pte_addr(&kernel_space[high], (u32_t)tableAddr >> 12);
		pde_set_p(&kernel_space[high], true);
		pde_set_rw(&kernel_space[high], true);
		pde_set_us(&kernel_space[high], true);
		for (; mid <= 0x3ff; mid++) {
			u32_t pageNum = (high << 10) + mid;
			tableAddr[mid].pte_attribute = 0;
			pte_set_page_addr(&tableAddr[mid], (u32_t)pageNum);
			pte_set_p(&tableAddr[mid], true);
			pte_set_rw(&tableAddr[mid], true);
			pte_set_us(&tableAddr[mid], true);
		}
	}
}

extern void set_paging();
void open_paging_model()
{
	paging_init_kernel_space();
	set_paging();
}

