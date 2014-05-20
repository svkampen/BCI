#ifndef _INTERPRET_H_
#define _INTERPRET_H_
struct return_struct {
	int iterations;
	char* output;
};

struct return_struct *run_tape(char* tape);

#endif
