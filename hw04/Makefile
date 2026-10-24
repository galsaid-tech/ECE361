
CC = gcc
CFLAGS = -std=c11 -Wall -Wextra

OBJS = tally.o

all: events

events: events.o $(OBJS)
	$(CC) $(CFLAGS) events.o $(OBJS) -o events

events.o: events.c tally.h
	$(CC) $(CFLAGS) -c events.c

tally.o: tally.c tally.h
	$(CC) $(CFLAGS) -c tally.c

tests/test_tally: tests/test_tally.c $(OBJS)
	$(CC) $(CFLAGS) -I. tests/test_tally.c $(OBJS) -o tests/test_tally

test: tests/test_tally
	./tests/test_tally

clean:
	rm -f events events.o $(OBJS) tests/test_tally

.PHONY: all test clean
