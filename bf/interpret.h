#ifndef _INTERPRET_H_
#define _INTERPRET_H_

#ifdef FMATH
int sse_round(double val);
#endif
struct return_struct {
	int iterations;
	char* output;
};

struct return_struct *run_tape(char* tape);

#endif