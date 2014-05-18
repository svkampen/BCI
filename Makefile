CC=clang
CFLAGS=-march=native -g -ggdb -Ilist -std=gnu99
LDFLAGS=-lm
FILES=bf/*.c list/list.c list/stack.c

all:
	$(CC) $(CFLAGS) $(LDFLAGS) -o bci $(FILES)
