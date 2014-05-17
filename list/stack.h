#ifndef _STACK_H_
#define _STACK_H_
#include <inttypes.h>
#include "list.h"

struct stack_t {
	uint8_t isEmpty;
	struct double_link *list;
};

int stack_pop(struct stack_t *stack);
int stack_peek(struct stack_t *stack);
int stack_push(struct stack_t *stack, int val);

void stack_free(struct stack_t *stack);
struct stack_t *stack_create_with(int init_val);
struct stack_t *stack_create();

#endif