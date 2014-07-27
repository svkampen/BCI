#include "preprocess.h"
#include "util.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "magic.h"

char *preprocess(char *input) {
	static char* procedures[52];

    
	char *nums = calloc(10, sizeof(char));
    int num_index = 0;
    int num = 0;
    char to_repeat = 0;
    char *to_repeat_s = NULL;
    int repeat_start = 0;

    int processed_length = strlen(input)+1;


	char *expanded = calloc(processed_length, sizeof(char));
	char *processed = NULL;
    int processed_index = 0;
    char *x = NULL;
	
	int changes = 0;

    // First, do expansions and repetitions.
	
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
				// printf("%c\n", input[input_pointer]);
                nums[num_index++] = input[input_pointer++];
            }
            // We are now at the end of the numbers. Decrease the input pointer,
            // because it will be incremented again next in the for loop, and
            // if we don't decrement now we will miss a character.

            --input_pointer;
            if (to_repeat == '+') {
                processed_index += sprintf(expanded+processed_index, "%s", nums);
                continue;
            }

			++changes;
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
				expanded = reallocate(expanded, processed_length);
                for(; num != 0; --num) {
                    expanded[processed_index++] = to_repeat;
                }
            } else {
                repeat_start = index_in(input, '{')+1;
                // Replace the { with \x02 so that when there is a new bracket repetition,
                // it does not mistake the { of this brep with the beginning of the other brep.
                input[repeat_start-1] = PREVIOUSLY_BRACKET_OPEN;
                to_repeat_s = get_part(input, repeat_start, input_pointer-strlen(nums));
                processed_length += strlen(to_repeat_s)*(num-1);
                expanded = reallocate(expanded, processed_length);
				memset(expanded+processed_index, 0, processed_length-processed_index);
				for (; num != 0; --num) {
                    processed_index += sprintf(expanded+processed_index, "%s", to_repeat_s);
                }
				free(to_repeat_s);
            }
		} else if (input[input_pointer] == '{' || input[input_pointer] == '}') {
			continue;
		} else {
			expanded[processed_index++] = input[input_pointer];
		}
        memset(nums, 0, 10);
        num = 0;
        num_index = 0;
	}
	processed = calloc(processed_length, sizeof (char));
	processed_index = 0;

	for (int input_pointer = 0; expanded[input_pointer] != '\0'; ++input_pointer) {
	    if (expanded[input_pointer] == '(') {
			++changes;
			// Get the character that identifies the procedure. Increment the input pointer to skip past '('
			// Increment the input pointer at the end so that you skip past the identifier.
			char name = expanded[++input_pointer];
			if (!isalpha(name)) {
				fprintf(stderr, "Error: line %d: procedure name '%c' is not an alphabetic character ([A-Za-z])\n",
						input_pointer, name);
				exit(-2);
			}
			
			int index;
			if (name >= 97 && name <= 122) {
				// lowercase letter.
				index = name-71;
			} else {
				index = name-65;
			}
			++input_pointer;
			
			// The length is the distance from here (expanded+input_pointer) to ')'.
			int len = index_in(expanded+input_pointer, ')');
			char *part = get_part(expanded+input_pointer, 0, len);
			procedures[index] = part;
			input_pointer += len;

			// reallocate the pointer to remove the excess memory usage.
			// safe, because you can always free a part of the memory.
			processed = reallocate(processed, processed_length-(3+len));
		} else if (expanded[input_pointer] == '*') {
			// procedure call
			++changes;
			char name = expanded[input_pointer-1];
				
			if (!isalpha(name)) {
				fprintf(stderr, "Error: line %d: procedure name '%c' in procedure call is not an alphabetic"
						"character ([A-Za-z])\n", input_pointer, name);
				exit(-2);
			}

			int index;
			if (name >= 97 && name <= 122) {
				// lowercase letter.
				index = name-71;
			} else {
				index = name-65;
			}

			char *to_insert = procedures[index];
		    if (processed_length < (1+processed_index+strlen(to_insert))) {
				processed_length *= 2;
				processed = reallocate(processed, processed_length);
				memset(processed+processed_index, 0, processed_length-processed_index);
			}
			processed_index += sprintf(processed+processed_index, "%s", to_insert);
        } else if (strcmp((x = get_part(expanded, input_pointer, input_pointer+3)), "[-]") == 0) {
            // Optimization. NULLIFY gets turned into *cell_p = 0; in the interpreter.
			++changes;
            processed[processed_index++] = NULLIFY;
            input_pointer += 2;
		} else if (isalpha(expanded[input_pointer])) {
			continue; // also do not copy these. (ASCII A-Z)
        } else {
			// copy the character.
			if (processed_length <= processed_index+1) {
				processed_length *= 2;
				processed = reallocate(processed, processed_length);
				memset(processed+processed_index, 0, processed_length-processed_index);
			}
            processed[processed_index++] = expanded[input_pointer];
        }
        if (x != NULL) {
            free(x);
            x = NULL;
        }
    }
    free(nums);
	free(expanded);
	processed = reallocate(processed, strlen(processed)+1);

	if (changes == 0) {
		free(processed);
		for (int i = 0; i < 52; ++i) {
			if (procedures[i]) {
				free(procedures[i]);
			}
		}
		return NULL;
	} else {
		free(input);
	    return processed;
	}
}
