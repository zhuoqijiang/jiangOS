#pragma once

#include "type.h"

void memcpy(void* dst, const void* src, u32_t size);
void memset(void* dst, int v, size_t size);
int strcmp(const char* a, const char* b);
char* strcat(char* dst, char* src);
int strlen(const char* str);
void itoas(int num,char* str,int radix);
int strfind(char* str, char c, size_t cnt);
