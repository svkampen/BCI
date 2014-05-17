#include "interpret.h"
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <math.h>
#include <string.h>
#include "stack.h"

#ifdef FMATH
#include <pmmintrin.h>
#include <immintrin.h>

int sse_round(double val) {
	return(_mm_cvtsd_si32(_mm_set_sd(val)));
}
#endif

struct return_struct *run_tape(char* tape) {
	int iterations = 0;
	char* output = calloc(2000, sizeof(char));
	char* output_p = output;
	int *cells = calloc(256, sizeof(int));
	int *cell_p = cells;

	int repetition_num = 0;
	int repetition_pos = -2;
	char to_repeat = 0;

	struct stack_t *loop_stack = stack_create();
	struct stack_t *var_stack = stack_create();

	for (int input_pointer = 0; tape[input_pointer] != '\0'; ++input_pointer) {
		++iterations;
		//fprintf(stderr, "Handling %c\n", tape[input_pointer]);
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
			output_p += sprintf(output_p, "%c", *cell_p); break;
		case ',':
			*cell_p = getchar(); break;
		case '[':
			stack_push(loop_stack, input_pointer); break;
		case ']':
			if (*cell_p != 0) {
				input_pointer = stack_peek(loop_stack);
			} else {
				stack_pop(loop_stack);
			}
			break;
		/* Start non-standard extensions */
		case '@':
			stack_push(var_stack, *cell_p); break;
		case '#':
			*cell_p = stack_peek(var_stack); break;
		case '$':
			*cell_p = stack_pop(var_stack); break;
		case ':':
			output_p += sprintf(output_p, "%d", *cell_p); break;
		case '\'':
			*cell_p = tape[input_pointer+1]; break;
		case '^':
			*cell_p = (*cell_p)*(*cell_p); break;
		case '/':
			#ifdef FMATH
			*cell_p = sse_round(sqrt(*cell_p));
			#else
			*cell_p = round(sqrt(*cell_p));
			#endif
			break;
		}
	}
	stack_free(loop_stack);
	stack_free(var_stack);
	var_stack = loop_stack = NULL;
	free(cells);
	
	size_t size = strlen(output);
	output_p = NULL;
	output = realloc(output, size);
	struct return_struct *s = malloc(sizeof(struct return_struct));
	s->iterations = iterations;
	s->output = output;
	return s;
}