
CC = cc
CFLAGS = -std=c11 -Wall -Wextra -g
SANFLAGS = -fsanitize=address,undefined -fno-omit-frame-pointer

.PHONY: all test memcheck clean

all: treestats test_bst

treestats: treestats.c bst.c bst.h
	$(CC) $(CFLAGS) -o $@ treestats.c bst.c

test_bst: tests/test_bst.c bst.c bst.h
	$(CC) $(CFLAGS) -o $@ bst.c tests/test_bst.c

test: test_bst
	./test_bst

memcheck:
	$(CC) $(CFLAGS) $(SANFLAGS) -o test_bst_mem bst.c tests/test_bst.c
	./test_bst_mem

clean:
	rm -f treestats test_bst test_bst_mem
