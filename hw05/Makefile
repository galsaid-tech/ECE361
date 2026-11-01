
CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -pedantic

all: lock

lock: main.o lock.o
	$(CC) $(CFLAGS) main.o lock.o -o lock

main.o: main.c lock.h
	$(CC) $(CFLAGS) -c main.c

lock.o: lock.c lock.h
	$(CC) $(CFLAGS) -c lock.c

tests/test_lock: tests/test_lock.c lock.o lock.h
	$(CC) $(CFLAGS) -I. tests/test_lock.c lock.o -o tests/test_lock

test: tests/test_lock
	./tests/test_lock

clean:
	rm -f lock main.o lock.o tests/test_lock

.PHONY: all test clean
