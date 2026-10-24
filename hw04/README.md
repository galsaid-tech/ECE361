# ECE 361 – Homework 4

## About This Homework

This homework is about creating a tally using C.

The tally counts how many times each name appears.

## Files

- `tally.h` – Contains the function declarations.
- `tally.c` – Contains the tally functions.
- `events.c` – Reads and counts names.
- `tests/test_tally.c` – Tests the functions.
- `data/door.txt` – Example input file.
- `Makefile` – Builds and tests the program.

## How to Build

Open the terminal inside `hw04` and run:

```bash
make
```

## How to Test

```bash
make test
```

My test result:

23 passed, 0 failed.

## How to Run

```bash
./events < data/door.txt
```

Expected output:

```text
close 2
locked 1
open 3
names: 3, lines skipped: 0
```

## How to Clean

```bash
make clean
```

## Part 3: Opaque Type

The `tally_t` type is opaque because `tally.h` does not show what is inside `struct tally`. Only `tally.c` knows its members. This means other files cannot directly change the tally's data. They must use the functions in `tally.h`. This helps protect the data and prevents mistakes.


## Compiler Error

I added this line to events.c:

printf("%d\n", t->size);

The compiler showed this error:

events.c:8:21: error: incomplete definition
of type 'tally_t' (aka 'struct tally')

This happened because tally.h only declares
struct tally. Its members are hidden in tally.c.

The opaque type prevents other files from
accessing or changing the tally data directly.
This helps protect the data and prevents mistakes.

