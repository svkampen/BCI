#include "util.h"
#include <stdlib.h>
#include <string.h>

int index_in(char* string, char part) {
    char *tmp = strchr(string, part);
    int tmp_i=0;
    for(; (tmp-tmp_i) != string; ++tmp_i);
    return tmp_i;
}

char* get_part(char* string, int x, int y) {
    char *tmp = calloc((1+(y-x)), sizeof(char));
    strncpy(tmp, (string+x), y-x);
    return tmp;
}

