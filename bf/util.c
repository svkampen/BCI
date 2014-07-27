#include "util.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int index_in(char* string, char part) {
    char *tmp = strchr(string, part);
    int tmp_i=0;
    for(; (tmp-tmp_i) != string; ++tmp_i);
    return tmp_i;
}

char* get_part(char* string, int x, int y) {
    char *tmp = calloc(1+(y-x), sizeof(char));
    strncpy(tmp, (string+x), y-x);
    return tmp;
}

int find_loop_end(char* string) {
	int n = 0;
	int index = 0;
	do {
		if (string[index] == '[') {
			n++;
		} else if (string[index] == ']') {
			n--;
		} else if (string[index] == '?') {
			n--;
		}
		index++;
	} while (n != 0);

	return index-1;
}

void *reallocate(void *ptr, size_t size) {
	void *tmp = NULL;
	tmp = realloc(ptr, size);
	if (tmp == NULL) {
		// Reallocate failed.
		// Spit out an error message and quit.
		fprintf(stderr, "Error while reallocating memory: pointer returned by realloc was NULL.\n");
		exit(-1);
	} else {
    	return tmp;
	}
}
