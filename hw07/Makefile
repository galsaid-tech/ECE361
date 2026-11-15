CC = cc
CFLAGS = -std=c11 -Wall -Wextra -Wpedantic -g
SANFLAGS = -fsanitize=address,undefined -fno-omit-frame-pointer

.PHONY: all test memcheck clean

all: balance

balance: balance.c istack.c istack.h
	$(CC) $(CFLAGS) $(EXTRAFLAGS) balance.c istack.c -o balance

tests/test_istack: tests/test_istack.c istack.c istack.h
	$(CC) $(CFLAGS) $(EXTRAFLAGS) tests/test_istack.c istack.c -o tests/test_istack

test: balance tests/test_istack
	./tests/test_istack
	sh ./regress.sh

memcheck:
	$(MAKE) clean
	$(MAKE) EXTRAFLAGS="$(SANFLAGS)" test

clean:
	rm -f balance tests/test_istack
