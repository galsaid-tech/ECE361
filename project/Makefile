
CC = gcc
CFLAGS = -std=c11 -Wall -Wextra

OBJS = main.o sensor.o fsm.o store.o aux.o report.o iom361_r4.o

all: cellar

cellar: $(OBJS)
	$(CC) $(CFLAGS) -o cellar $(OBJS)

main.o: main.c iom361_r4.h sensor.h fsm.h store.h aux.h report.h
	$(CC) $(CFLAGS) -c main.c

sensor.o: sensor.c sensor.h
	$(CC) $(CFLAGS) -c sensor.c

fsm.o: fsm.c fsm.h
	$(CC) $(CFLAGS) -c fsm.c

store.o: store.c store.h
	$(CC) $(CFLAGS) -c store.c

aux.o: aux.c aux.h
	$(CC) $(CFLAGS) -c aux.c

report.o: report.c report.h
	$(CC) $(CFLAGS) -c report.c

iom361_r4.o: iom361_r4.c iom361_r4.h
	$(CC) $(CFLAGS) -c iom361_r4.c

clean:
	rm -f $(OBJS) cellar

.PHONY: all clean
