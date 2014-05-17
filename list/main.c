#include "stack.h"
#include <assert.h>

int main() {
	struct stack_t *stack = stack_create(5);
	printf("%d\n", stack_peek(stack));
	stack_push(stack, 4);
	printf("%d\n", stack_pop(stack));
	printf("%d\n", stack_peek(stack));
	if (stack->list == NULL) {
		printf("true\n");
	} else {
		printf("false\n");
	}
	stack_pop(stack);
	stack_free(stack);
}