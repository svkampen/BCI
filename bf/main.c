/* BCI - Boonfuck C Interpreter
 * usage: bci <boonfuck>
 * run with -s to enable strict mode 
 * run with -b to enable brainfuck mode
 * (C) 2014 Sam van Kampen
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interpret.h"
#include "preprocess.h"
#include "magic.h"

char const HELP[] = "\nHelp\n\n\
					 .,+-<>[] are like in Brainfuck.\n\
					 \n\
					 == The Stack\n\
					 @ pushes, # peeks and $ pops the stack.\n\
					 \n\
					 == Repetition\n\
					 Put a number after an operator to repeat it.\n\
					 Example: +65\n\
					 \n\
					 Enclose a group of operators in brackets and put a\n\
					 number behind them to repeat them: {.>}5\n\
					 \n\
					 ";

char *prepro(char *input) {
	char *processed = input;
	char *tmp;

	do {
		tmp = processed;
		processed = preprocess(processed);	
	} while (processed != NULL);
	return tmp;
}

char *interpret(char *string, uint8_t flags) {
	char *processed = strdup(string);
	char *input;

	do {
		input = processed;
		processed = preprocess(processed);
	} while (processed);	
    char *tmp = run_tape(input, flags);
	free(input);
	return tmp;
}

char *interpret2(char *string, uint8_t flags) {
	return interpret(strdup(string), flags);
}

int main(int argc, char* argv[]) {
	if (argc == 1) {
		fprintf(stderr, "BCI - The Boonfuck C Interpreter\n");
		fprintf(stderr, "Usage: bci [flags] <boonfuck>\n");
		fprintf(stderr, "run with -p to only preprocess, -b to enable brainfuck mode.\n");
		fprintf(stderr, HELP);
		return -1;
	}

	char *processed = NULL;

	if (strcmp(argv[1], "-p") == 0) {
		processed = prepro(strdup(argv[2])); 
		printf("%s\n", processed);
		free(processed);
	} else if (strcmp(argv[1], "-q") == 0) {
		interpret(strdup(argv[2]), FLG_QUIET | FLG_PRNT);
	} else {
		interpret(strdup(argv[1]), FLG_PRNT);
	}
	return 0;
}
