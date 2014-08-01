#ifndef _UTIL_H_
#define _UTIL_H_
#include <stdlib.h>
char* get_part(char* string, int x, int y);
int index_in(char* string, char part);
int find_loop_end(char* string);
int find_bracket_begin(char* string);
void *reallocate(void *ptr, size_t size) __attribute__ ((warn_unused_result));
#endif
