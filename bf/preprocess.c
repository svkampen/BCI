#include "preprocess.h"
#include "util.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

char* preprocess(char* input) {
	char* nums = calloc(10, sizeof(char));
	int num_index = 0;
	int num = 0;
	char to_repeat = 0;
    char *to_repeat_s;
    int repeat_start = 0;

	char* processed = calloc(2000, sizeof(char));
	int processed_index = 0;

	for(int input_pointer = 0; input[input_pointer] != '\0'; ++input_pointer) {
        if (isdigit(input[input_pointer])) {
			// Number.
			to_repeat = input[input_pointer-1];
		    nums[num_index++] = input[input_pointer++];
		    while (isdigit(input[input_pointer])) {
			    nums[num_index++] = input[input_pointer++];
   			}
    		--input_pointer;
	    	num = atoi(nums)-1;
            if (to_repeat != '}') {
			    for(; num != 0; --num) {
				    processed[processed_index++] = to_repeat;
			    }
            } else {
                repeat_start = index_in(input, '{')+1;
                input[repeat_start-1] = '\x02';
                to_repeat_s = get_part(input, repeat_start, input_pointer-1);
                for (; num != 0; --num) {
                    processed_index += sprintf(processed+processed_index, "%s", to_repeat_s);
                }
            }
        } else if (input[input_pointer] == '{' || input[input_pointer] == '}') {
            continue; //do not copy these characters.
		} else {
			processed[processed_index++] = input[input_pointer];
		}
		memset(nums, 0, 10);
		num = 0;
		num_index = 0;
	}
	// Now reallocate to remove extra memory usage.
	processed = realloc(processed, processed_index+1);
	free(nums);
	return processed;
}
