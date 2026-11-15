# ECE 361 — Homework 7

## What This Program Does

`istack` stores integers in a dynamic stack. The stack starts with room for 8 values and doubles its capacity when full.

`balance` checks brackets `()`, `[]`, and `{}` from standard input. It checks brackets even inside comments and strings.

## Build and Run

Open the terminal in the `hw07` folder.

```sh
make
./balance < inputs/balanced.txt
```

Example output:

```text
balanced: 3 pairs, deepest 3
```

The program returns 0 if all brackets are balanced and 1 if there is an error.

## Tests

Run these commands:

```sh
make test
make memcheck
make clean
```

- `make test` runs the stack and bracket tests.
- `make memcheck` checks for memory errors.
- `make clean` removes generated files.

### Test Results

Before adding the temporary bugs:

- Stack tests: 9 passed, 0 failed.
- Regression tests: 8 passed, 0 failed.
- Memory check: Passed without reported errors.

On macOS, I also used the `leaks` command to test memory leaks.

## Part 4 — Memory Bugs

I tested three memory bugs separately using AddressSanitizer and the `leaks` command on my Mac.

### 1. Destroy the Same Stack Twice

I created a stack and called `istack_destroy(s)` twice.

**Error:**

```text
ERROR: AddressSanitizer: heap-use-after-free
```

**Explanation:**

The first destroy frees the stack memory. The second destroy tries to access memory that was already freed.

### 2. Push After Destroying the Stack

I created a stack, destroyed it, and then called `istack_push(s, 4)`.

**First error line:**

```text
==12265==ERROR: AddressSanitizer: heap-use-after-free on address 0x603000001c00 at pc 0x000104e48b54
```

**Explanation:**

The program tries to use the stack after its memory was freed. AddressSanitizer detects this error.

### 3. Remove istack_destroy from balance.c

I removed `istack_destroy(s)` from `balance.c` and ran the memory leak test on my Mac.

I used this command:

```sh
leaks --quiet --atExit -- ./balance < inputs/balanced.txt
```

**First leak summary line:**

```text
Process 12644: 2 leaks for 80 total leaked bytes.
```

**Explanation:**

The program did not free the stack and its array. This caused 2 memory leaks (80 bytes).

## Final Check

After testing each bug, I restored the original code.

Final commands:

```sh
make clean
make test
make memcheck
make clean
```

The final code must pass all tests without memory errors.

## Files

- `istack.h` — Stack interface.
- `istack.c` — Stack functions.
- `balance.c` — Bracket checker.
- `tests/test_istack.c` — Stack tests.
- `inputs/` — Test input files.
- `expected/` — Expected results.
- `regress.sh` — Regression tests.
- `Makefile` — Build and test commands.
- `README.md` — Homework explanation and memory tests.
- `AI_USAGE.md` — AI usage statement.