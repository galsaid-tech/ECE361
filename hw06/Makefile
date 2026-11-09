
CC = gcc
CFLAGS = -std=c11 -Wall -Wextra
LDLIBS = -lm

OBJS = main.o sensor.o panel.o scenario.o iom361_r4.o

all: replay

replay: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o replay $(LDLIBS)

main.o: main.c sensor.h panel.h scenario.h iom361_r4.h
	$(CC) $(CFLAGS) -c main.c

sensor.o: sensor.c sensor.h iom361_r4.h
	$(CC) $(CFLAGS) -c sensor.c

panel.o: panel.c panel.h iom361_r4.h
	$(CC) $(CFLAGS) -c panel.c

scenario.o: scenario.c scenario.h
	$(CC) $(CFLAGS) -c scenario.c

iom361_r4.o: iom361_r4.c iom361_r4.h
	$(CC) $(CFLAGS) -c iom361_r4.c


tests/test_hw06: tests/test_hw06.c sensor.o panel.o scenario.o iom361_r4.o
	$(CC) $(CFLAGS) -I. tests/test_hw06.c sensor.o panel.o scenario.o iom361_r4.o -o tests/test_hw06 $(LDLIBS)

test: replay tests/test_hw06
	./tests/test_hw06
	./regress.sh

clean:
	rm -f $(OBJS) replay tests/test_hw06

.PHONY: all test clean
