# ECE 361 – Homework 3

## About This Homework

This homework is about pointers in C.

I created four functions to work with float numbers and arrays.

## Functions

### 1. swap_float

This function swaps two float numbers using pointers.

For example, if the numbers are 5 and 9, they become 9 and 5.

### 2. minmax

This function finds the smallest and largest numbers in an array.

It returns 0 when it works and -1 when the input is invalid.

### 3. find_above

This function finds the first number greater than a threshold.

It returns the address of that number, or NULL if no number is found.

### 4. reverse

This function reverses the order of numbers in an array.

It uses two pointers and does not need another array.

## Files

- `ptr.h` contains the function declarations.
- `ptr.c` contains the function code.
- `tests/test_ptr.c` contains the tests.
- `Makefile` builds and runs the tests.

## How to Run

Open the Terminal inside the `hw03` folder.

To run the tests:

```bash
make test
```

To remove the compiled test program:

```bash
make clean
```

## Test Results

I tested all four functions and some special cases.

All 13 tests passed.

## What I Learned

I learned how to use pointers to change values, find numbers in arrays, and reverse arrays. I also learned how to test my functions.