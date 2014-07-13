#include "preprocess.h"
#include "util.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "magic.h"

char* preprocess(char* input) {
    char* nums = calloc(10, sizeof(char));
    int num_index = 0;
    int num = 0;
    char to_repeat = 0;
    char *to_repeat_s;
    int repeat_start = 0;

    int processed_length = strlen(input);

    char* processed = calloc(processed_length, sizeof(char));
    int processed_index = 0;

    for(int input_pointer = 0; input[input_pointer] != '\0'; ++input_pointer) {
        if (isdigit(input[input_pointer])) {
            // This is a number. Get the thing we have to repeat.
            to_repeat = input[input_pointer-1];
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
            if (to_repeat == '+') {
                processed_index += sprintf(processed+processed_index, "%s", nums);
                continue;
            }
            // Get the string of numbers as an int, and subtract one.
            // The subtraction is done because one of the repetitions has
            // already been done.
            num = atoi(nums)-1;
            
            // Okay. If the thing to repeat isn't a '{' (which would
            // indicate a bracket repetition), just repeat it n times.
            //
            // If not, figure out what to repeat and use sprintf to do
            // the repetition in a /really nice way/.
            if (to_repeat != '}') {
                processed_length += num;
                processed = realloc(processed, processed_length);
                for(; num != 0; --num) {
                    processed[processed_index++] = to_repeat;
                }
            } else {
                repeat_start = index_in(input, '{')+1;
                // Replace the { with \x02 so that when there is a new bracket repetition,
                // it does not mistake the { of this brep with the beginning of the other brep.
                input[repeat_start-1] = PREVIOUSLY_BRACKET_OPEN;
                to_repeat_s = get_part(input, repeat_start, input_pointer-strlen(nums));
                processed_length += strlen(to_repeat_s)*(num-1);
                processed = realloc(processed, processed_length);
                for (; num != 0; --num) {
                    processed_index += sprintf(processed+processed_index, "%s", to_repeat_s);
                }
            }
        } else if (strcmp(get_part(input, input_pointer, input_pointer+3), "[-]") == 0) {
            // optimize this. It's easy!
            processed[processed_index++] = NULLIFY;
            input_pointer += 2;
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
