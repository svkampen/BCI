#ifndef _UTIL_H_
#define _UTIL_H_
#include <string.h>

char* get_part(char* string, int x, int y);
int index_in(char* string, char part);
void* reallocate(void* pointer, size_t new_size);
int round(double val);
#endif
