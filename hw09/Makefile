
CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -Werror -g
TARGET = tests/test_ht

.PHONY: all test memcheck clean

all: $(TARGET)

$(TARGET): ht.c ht.h tests/test_ht.c
	$(CC) $(CFLAGS) -o $(TARGET) ht.c tests/test_ht.c

test: $(TARGET)
	./$(TARGET)

memcheck: $(TARGET)
	@if command -v valgrind >/dev/null 2>&1; then \
		valgrind --leak-check=full --error-exitcode=1 ./$(TARGET); \
	else \
		echo "Valgrind not found. Using AddressSanitizer."; \
		$(CC) $(CFLAGS) -fsanitize=address -fno-omit-frame-pointer \
		-o tests/test_ht_asan ht.c tests/test_ht.c; \
		./tests/test_ht_asan; \
		rm -f tests/test_ht_asan; \
	fi

clean:
	rm -f $(TARGET) tests/test_ht_asan
