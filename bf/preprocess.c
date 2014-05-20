#include "preprocess.h"
#include "util.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>

char *nums;
int num_index;
int num;
char to_repeat_c;
char *to_repeat_s;
int repeat_start;
int processed_length;
char *processed;
int processed_index;
int input_pointer;

char* preprocess(char* input) {
    nums = calloc(10, sizeof(char));
    num_index = 0;
    num = 0;
    to_repeat_c = 0;
    repeat_start = 0;

    processed_length = strlen(input);

    processed = calloc(processed_length, sizeof(char));
    processed_index = 0;

    for(; input[input_pointer] != '\0'; ++input_pointer) {
        if (isdigit(input[input_pointer])) {
            // This is a number. Get the thing we have to repeat.
            to_repeat_c = input[input_pointer-1];
            // Set the num_indexth character of the nums string
            // to the number at this point of the input string.
            // 
            // Increase both num_index and input_pointer.
            nums[num_index++] = input[input_pointer++];
            // While the input_pointer points to a number,
            // add it to the nums string.
            while (isdigit(input[input_pointer])) {
                nums[num_index++] = input[input_pointer++];
            }
            // We are now at the end of the numbers. Decrease the input pointer,
            // because it will be incremented again next in the for loop, and
            // if we don't decrement now we will miss a character.

            --input_pointer;
            // Get the string of numbers as an int, and subtract one.
            // The subtraction is done because one of the repetitions has
            // already been done.
            num = atoi(nums)-1;
            
            // Okay. If the thing to repeat isn't a '{' (which would
            // indicate a bracket repetition), just repeat it n times.
            //
            // If not, figure out what to repeat and use sprintf to do
            // the repetition in a /really nice way/.
            if (to_repeat_c != '}') {
                processed_length += num-1;
                processed = reallocate(processed, processed_length);
                for(; num != 0; --num) {
                    processed[processed_index++] = to_repeat_c;
                }
            } else {
                repeat_start = index_in(input, '{')+1;
                input[repeat_start-1] = '\x02';
                to_repeat_s = get_part(input, repeat_start, input_pointer-strlen(nums));
                processed_length += strlen(to_repeat_s)*(num-1);
                processed = reallocate(processed, processed_length);
                for (; num != 0; --num) {
                    processed_index += sprintf(processed+processed_index, to_repeat_s);
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
    free(nums);
    return processed;
}
