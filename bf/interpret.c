#include "interpret.h"
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <ctype.h>
#include <math.h>
#include <string.h>
#include "../list/stack.h"
#include "util.h"
#include "magic.h"

void run_tape(char* tape) {
	unsigned long long iterations = 0;
	int *cells = calloc(1024, sizeof *cells);
	int *cell_p = cells;

	struct stack_t *loop_stack = stack_create();
	struct stack_t *var_stack = stack_create();

	for (int input_pointer = 0; tape[input_pointer] != '\0'; ++input_pointer) {
		++iterations;
		// fprintf(stderr, "Handling %c\n", tape[input_pointer]);
		switch(tape[input_pointer]) {
		case '+':
			(*cell_p) += 1; break;
		case '-':
			(*cell_p) -= 1; break;
		case '>':
			cell_p++; break;
		case '<':
			cell_p--; break;
		case '.':
			putchar(*cell_p); fflush(stdout); break;
		case ',':
			*cell_p = getchar(); break;
		case '[':
			if (*cell_p == 0) {
				input_pointer += find_loop_end((tape+input_pointer));
				break;
			}
			stack_push(loop_stack, input_pointer); break;
		case ']':
			input_pointer = stack_pop(loop_stack)-1;
			continue;
		/* Start non-standard extensions */
		case '@':
			stack_push(var_stack, *cell_p); break;
		case '#':
			*cell_p = stack_peek(var_stack); break;
		case '$':
			*cell_p = stack_pop(var_stack); break;
		case ':':
			printf("%d", *cell_p); break;
		case '\'':
			*cell_p = tape[input_pointer+1]; break;
		case '^':
			*cell_p = (*cell_p)*(*cell_p); break;
		case '/':
			// *cell_p = sse_round(sqrt(*cell_p));
			*cell_p = round(sqrt(*cell_p));
			break;
        case NULLIFY:
        	(*cell_p) = 0;
        	break;
		}
		if (isdigit(tape[input_pointer])) {
			{
				char *nums = calloc(10, 1);
				int num = 0;
				int num_index = 0;
				// Okay, fine, the only case wherein this can happen is '+'.
				nums[num_index++] = tape[input_pointer++];
    	        // While the input_pointer points to a number,
        	    // add it to the nums string.
            	while (isdigit(tape[input_pointer])) {
	                nums[num_index++] = tape[input_pointer++];
    	        }
        	    // We are now at the end of the numbers. Decrease the input pointer,
            	// because it will be incremented again next in the for loop, and
	            // if we don't decrement now we will miss a character.

    	        --input_pointer;
        	    // Get the string of numbers as an int, and subtract one.
            	// The subtraction is done because one of the repetitions has
	            // already been done.
    	        num = atoi(nums)-1;
        	    *cell_p += num;
        	    free(nums);
        	}
		}
	}
	stack_free(loop_stack);
	stack_free(var_stack);
	var_stack = loop_stack = NULL;
	printf("\n\n\033[1;32mRan in %llu iterations.\n", iterations);
	free(cells);
}
