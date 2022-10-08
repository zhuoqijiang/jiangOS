#include "kernel/base/string.h"
#include "kernel/base/type.h"
void memcpy(void* dst, const void* src, u32_t size)
{
	u8_t* c_dst = (u8_t*)dst;
	const u8_t* c_src = (u8_t*) src;
	for (int i = 0; i < size; i++) {
		*c_dst = *c_src;
		c_src++;
		c_dst++;
	}
	return;
}


void memset(void* dst, int v, size_t size)
{
	u8_t* c_dst = (u8_t*)dst;
	for (int i = 0; i < size; i++) {
		*c_dst = v;
		c_dst++;
	}
}


int strcmp(const char* a,const char* b)
{
	int cursor = 0;
	while(true) {
		if (a[cursor] == 0 && b[cursor] == 0) {
			return 0;
		}
		if (a[cursor] == 0) {
			return -1;
		}
		if (b[cursor] == 0) {
			return 1;
		}
		if (a[cursor] < b[cursor]) {
			return -1;
		}
		if (a[cursor] > b[cursor]) {
			return 1;
		}
		cursor++;
	}
	return -2;
}


char* strcat(char* dst, char* src)
{
	char *dest = dst;
	while (*dest)
	{
		dest++;
	}
	while (*dest++ = *src++);
	return dest;
}

int strlen(const char* str)
{
	int len = 0;
	while (*str++) {
		len++;
	}
	return len;
}

void itoas(int num,char* str,int radix)
{
    char index[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    unsigned unum;
    int i = 0, j, k;
    if (radix == 10 && num < 0) {
        unum = (unsigned) - num;
        str[i++] = '-';
    }
    else {
        unum = (unsigned)num;
    }
    do {
        str[i++] = index[unum % (unsigned)radix];
        unum /= radix;
    } while(unum);
    str[i] = '\0';
    if (str[0] == '-')
        k = 1;
    else
        k = 0;

    char temp;
    for (j = k; j <= (i - 1) / 2; j++) {
        temp = str[j];
        str[j] = str[i - 1 + k - j];
        str[i - 1 + k - j] = temp;
    }
}

int strfind(char* str, char c, size_t cnt)
{
	for (int i = 0; i < strlen(str); i++) {
		if (str[i] == c) {
			if (--cnt == 0) {
				return i;
			}
		}
	}
	return -1;
}
