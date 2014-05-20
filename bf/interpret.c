#include "interpret.h"
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#ifdef __GNUC__
#include <math.h>
#endif
#include <string.h>
#include "stack.h"
#include "util.h"

#define REALLOC_SIZE 128

int iterations;
char* output;
char* output_p;	
int *cells;
int *cell_p;

int repetition_num;
int repetition_pos;
char to_repeat;

struct stack_t *loop_stack;
struct stack_t *var_stack;

int input_pointer;
struct return_struct *s;

unsigned int size;

struct return_struct *run_tape(char* tape) {
	iterations = 0;
	output = calloc(256, sizeof(char));
	output_p = output;

	cells = calloc(256, sizeof(int));
	cell_p = cells;

	loop_stack = stack_create();
	var_stack = stack_create();
	input_pointer = 0;

	s = malloc(sizeof(struct return_struct));
	for (; tape[input_pointer] != '\0'; ++input_pointer) {
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
        case '?':
            if (*cell_p != 0) {
                input_pointer = index_in(tape, '!');
            } else {
                tape[index_in(tape, '!')] = '\x03';
            }
            break;
        case '\x03':
            input_pointer = index_in(tape, '*');
            tape[input_pointer] = '\x04';
            break;
        case '~':
            if (*cell_p < 0) {
                *(cell_p+1) = 1;
            } else {
                *(cell_p+1) = 0;
            } break;
        case '&':
            if (*cell_p > 0) {
                *(cell_p+1) = 1;
            } else {
                *(cell_p+1) = 0;
            } break;
		}
	}
	stack_free(loop_stack);
	stack_free(var_stack);
	var_stack = NULL;
	loop_stack = NULL;
	free(cells);
	
	size = strlen(output);
	output_p = NULL;
	output = reallocate(output, size);
	s->iterations = iterations;
	s->output = output;
	return s;
}
