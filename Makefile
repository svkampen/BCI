CC=cl65
CFLAGS=-Ilist -Ibf
LDFLAGS=-lm
FILES=bf/*.c list/list.c list/stack.c

all:
	$(CC) $(CFLAGS) $(LDFLAGS) -o bci $(FILES)
