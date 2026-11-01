
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
	rm -f lock main.o lock.o check.o tests/test_lock tests/test_check


.PHONY: all test bonus clean

check.o: check.c check.h lock.h
	$(CC) $(CFLAGS) -c check.c

tests/test_check: tests/test_check.c check.o lock.o check.h lock.h
	$(CC) $(CFLAGS) -I. tests/test_check.c check.o lock.o -o tests/test_check

bonus: tests/test_check
	./tests/test_check
