#include "stack.h"
#include <assert.h>

/* Peek at the stack top.
 *
 * @param stack The stack to peek at.
 * @return The value at the top of the stack.
 */
int stack_peek(struct stack_t *stack) {
	return stack->list->value;
}

/* Pop an item from the stack.
 *
 * @param stack The stack to pop from.
 * @return The value that was popped.
 */
int stack_pop(struct stack_t *stack) {
	int val = stack->list->value;
	if (stack->list->next == NULL) {
		free(stack->list);
		stack->list = NULL;
	} else {
		stack->list = stack->list->next;
		free(stack->list->prev);
		stack->list->prev = NULL;
	}
	return val;
}

/* Push an item onto the stack.
 *
 * @param stack The stack to push onto.
 * @param val The value to push onto the stack.
 */
int stack_push(struct stack_t *stack, int val) {
	LINK_CREATE(tmp);
	tmp->value = val;
	if (stack->list != NULL) {
		stack->list->prev = tmp;
		tmp->next = stack->list;
	}
	stack->list = tmp;
	return 0;
}

/* Create a stack, and initialize the top value to
 * init_val.
 *
 * @param init_val The top value of the new stack.
 * @return The new stack.
 */
struct stack_t *stack_create_with(int init_val) {
	struct stack_t *tmp = calloc(1, sizeof(struct stack_t));
	tmp->list = calloc(1, sizeof(struct double_link));
	tmp->list->value = init_val;
	tmp->isEmpty = 0;
	return tmp;
}

struct stack_t *stack_create() {
	struct stack_t *tmp = calloc(1, sizeof(struct stack_t));
	tmp->list = NULL;
	return tmp;
}

/* Free a stack.
 * This also frees the doubly linked list inside the stack.
 * 
 * Note: also works when the list inside the stack is already NULL.
 * Warning: Do not use the stack afterwards, because it points
 * to a piece of freed memory!
 *
 * @param stack The stack to free
 */
void stack_free(struct stack_t *stack) {
	struct double_link *elem;
	elem = stack->list;
	if (elem == NULL) {
		free(stack);
		stack = NULL;
		return;
	}

	while(elem->prev != NULL) {
		elem = elem->prev;
	}
	
	// We are now at the head of the list.
	while(elem->next != NULL) {
		elem = elem->next;
		free(elem->prev);
		elem->prev = NULL;
	}

	free(elem);
	stack->list = NULL;

	free(stack);
}