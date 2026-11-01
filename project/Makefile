
CC = gcc
CFLAGS = -std=c11 -Wall -Wextra
LDFLAGS =
LDLIBS = -lm

OBJS = main.o sensor.o fsm.o store.o aux.o report.o iom361_r4.o bits.o

TESTS = tests/test_store tests/test_fsm tests/test_sensor \
        tests/test_aux tests/test_report tests/test_status

all: cellar

cellar: $(OBJS)
	$(CC) $(CFLAGS) $(LDFLAGS) -o cellar $(OBJS) $(LDLIBS)

main.o: main.c bits.h iom361_r4.h sensor.h fsm.h store.h aux.h report.h
	$(CC) $(CFLAGS) -c main.c

sensor.o: sensor.c sensor.h iom361_r4.h
	$(CC) $(CFLAGS) -c sensor.c

fsm.o: fsm.c fsm.h sensor.h
	$(CC) $(CFLAGS) -c fsm.c

store.o: store.c store.h sensor.h
	$(CC) $(CFLAGS) -c store.c

aux.o: aux.c aux.h fsm.h
	$(CC) $(CFLAGS) -c aux.c

report.o: report.c report.h store.h
	$(CC) $(CFLAGS) -c report.c

bits.o: bits.c bits.h
	$(CC) $(CFLAGS) -c bits.c

iom361_r4.o: iom361_r4.c iom361_r4.h
	$(CC) $(CFLAGS) -c iom361_r4.c

tests/test_store: tests/test_store.c store.c store.h sensor.h
	$(CC) $(CFLAGS) $(LDFLAGS) -I. tests/test_store.c store.c -o tests/test_store $(LDLIBS)

tests/test_fsm: tests/test_fsm.c fsm.c fsm.h sensor.h
	$(CC) $(CFLAGS) $(LDFLAGS) -I. tests/test_fsm.c fsm.c -o tests/test_fsm $(LDLIBS)

tests/test_sensor: tests/test_sensor.c sensor.c sensor.h iom361_r4.c iom361_r4.h
	$(CC) $(CFLAGS) $(LDFLAGS) -I. tests/test_sensor.c sensor.c iom361_r4.c -o tests/test_sensor $(LDLIBS)

tests/test_aux: tests/test_aux.c aux.c aux.h fsm.h sensor.h
	$(CC) $(CFLAGS) $(LDFLAGS) -I. tests/test_aux.c aux.c -o tests/test_aux $(LDLIBS)

tests/test_report: tests/test_report.c report.c report.h store.c store.h sensor.h
	$(CC) $(CFLAGS) $(LDFLAGS) -I. tests/test_report.c report.c store.c -o tests/test_report $(LDLIBS)

tests/test_status: tests/test_status.c bits.c bits.h
	$(CC) $(CFLAGS) $(LDFLAGS) -I. tests/test_status.c bits.c -o tests/test_status $(LDLIBS)

test: cellar $(TESTS)
	@passed=0; failed=0; \
	for program in $(TESTS); do \
		if ./$$program; then \
			passed=$$((passed + 1)); \
		else \
			failed=$$((failed + 1)); \
		fi; \
	done; \
	tmp=$$(mktemp) || exit 1; \
	for name in normal every_state thresholds alarm_recovery unexpected_switch midnight; do \
		if ./cellar scenarios/$$name.txt > "$$tmp" && \
		   diff -u scenarios/$$name.expected "$$tmp"; then \
			echo "$$name scenario - PASSED"; \
			passed=$$((passed + 1)); \
		else \
			echo "$$name scenario - FAILED"; \
			failed=$$((failed + 1)); \
		fi; \
	done; \
	if ./cellar scenarios/report_case.txt --report > "$$tmp" && \
	   diff -u scenarios/report_case.expected "$$tmp"; then \
		echo "report_case scenario - PASSED"; \
		passed=$$((passed + 1)); \
	else \
		echo "report_case scenario - FAILED"; \
		failed=$$((failed + 1)); \
	fi; \
	rm -f "$$tmp"; \
	echo "Tests passed: $$passed, failed: $$failed"; \
	test "$$failed" -eq 0

clean:
	rm -f $(OBJS) cellar $(TESTS)

.PHONY: all test clean
