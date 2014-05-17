#include "preprocess.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#define BETWEEN(X, Y, Z) X > Y && X < Z

extern "C" char* preprocess(char* input) {
	
	char* nums = calloc(10, sizeof(char));
	int num_index = 0;
	int num = 0;
	char to_repeat = 0;

	char* processed = calloc(2000, sizeof(char));
	int processed_index = 0;

	for(int input_pointer = 0; input[input_pointer] != '\0'; ++input_pointer) {
		if (BETWEEN(input[input_pointer], 47, 58)) {
			// Number.
			to_repeat = input[input_pointer-1];
			nums[num_index++] = input[input_pointer++];
			while (BETWEEN(input[input_pointer], 47, 58)) {
				nums[num_index++] = input[input_pointer++];
			}
			--input_pointer;
			num = atoi(nums)-1;
			for(; num != 0; --num) {
				processed[processed_index++] = to_repeat;
			}
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