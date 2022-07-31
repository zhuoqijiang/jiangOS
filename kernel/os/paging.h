#pragma once

#include "../base/type.h"
typedef struct {
	u32_t pde_attribute;
}pde_t;

void pde_set_pte_addr(pde_t *pde, u32_t addr);
u32_t pde_get_pte_addr(pde_t* pde);
void pde_set_p(pde_t* pde, bool flag);
void pde_set_rw(pde_t* pde, bool flag);
void pde_set_us(pde_t* pde, bool flag);


typedef struct {
	u32_t pte_attribute;
}pte_t;
void pte_set_page_addr(pte_t* pte, u32_t addr);
u32_t pte_get_page_addr(pte_t* pte);
void pte_set_p(pte_t* pte, bool flag);
void pte_set_rw(pte_t* pte, bool flag);
void pte_set_us(pte_t* pte, bool flag);

void open_paging_model();
