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

char *processed_main;
struct return_struct *rs;

int main(int argc, char* argv[]) {
	if (argc == 1) {
		fprintf(stderr, "BCI - The Boonfuck C Interpreter\n");
		fprintf(stderr, "Usage: bci [flags] <boonfuck>\n");
		fprintf(stderr, "run with -p to only preprocess, -b to enable brainfuck mode.\n");
        	return -1;
	}
    if (strcmp(argv[1], "-p") == 0) {
        printf("%s\n", preprocess(argv[2]));
        return 0;
    }
	processed_main = preprocess(argv[1]); 
	rs = NULL;
	rs = run_tape(processed_main);
	printf("%s\n", rs->output);
	free(rs->output);
	printf("\nRan in %d iterations.\n", rs->iterations);
	free(rs);
	free(processed_main);
	return 0;
}
