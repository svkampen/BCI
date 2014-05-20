#include "util.h"
#include <stdlib.h>
#include <stdio.h>

int round(double x) {
    return (int(x > 0 ? x + 0.5 : x - 0.5));

int index_in(char* string, char part) {
    char *tmp = strchr(string, part);
    int tmp_i=0;
    for(; (tmp-tmp_i) != string; ++tmp_i);
    return tmp_i;
}

char* get_part(char* string, int x, int y) {
    char *tmp = malloc((y-x)*sizeof(char));
    strncpy(tmp, (string+x), y-x);
    return tmp;
}

void *reallocate(void* pointer, size_t new_size) {
	void *tmp;
	tmp = realloc(pointer, new_size);
	if (tmp == NULL) {
		fprintf(stderr, "Error trying to reallocate %zu bytes of memory.", new_size);
		exit(-1);
	}
	return tmp;
}
