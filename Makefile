CC=gcc
CFLAGS=-march=native -g3 -ggdb3 -Ilist -Ibf -std=gnu99
LDFLAGS=-lm -lmcheck
FILES=bf/*.c list/list.c list/stack.c
SHAREDFLAGS=-shared -fPIC

all:
	$(CC) $(CFLAGS) $(LDFLAGS) -o bci $(FILES)

shared:
	$(CC) $(CFLAGS) $(SHAREDFLAGS) $(LDFLAGS) -o libbci.so $(FILES)

clean:
	rm bf/*.o; rm list/*.o;
