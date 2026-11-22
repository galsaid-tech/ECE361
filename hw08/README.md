
# ECE 361 – Homework 8

## About This Homework

This homework is about Binary Search Trees (BST).

I changed the original BST code into a library.
I also found and fixed a crash and tested how
insertion order changes the tree height.

## Files

- bst.h: Function declarations.
- bst.c: BST functions.
- treestats.c: Reads numbers and prints tree size and height.
- tests/test_bst.c: Tests the BST functions.
- Makefile: Builds and tests the program.
- AI_USAGE.md: Explains how I used AI.

## Build and Test

Build the program:

    make

Run the tests:

    make test

Check memory errors:

    make memcheck

Remove compiled files:

    make clean

## Original Search Bug

I tested bst_contains() with an empty tree.

The program crashed because it tried to read
current->data when current was NULL.

AddressSanitizer reported:

    SUMMARY: AddressSanitizer: SEGV bst.c:119 in bst_contains

The original program did not find this bug because
it only searched a tree that already had seven keys.

I fixed the bug by checking that current is not NULL
before reading its data.

After the fix, the empty tree test passed.

## BST Tests

I tested:
- NULL tree.
- Empty tree.
- Original seven keys.
- Searching for 31 and 15.
- Duplicate keys.
- Increasing keys.
- Destroying a full tree.

All tests passed.

## Tree Statistics

### Increasing Order

Command:

    seq 1 1023 | ./treestats

Output:

    n 1023, height 1023

### Shuffled Order

Command:

    seq 1 1023 | sort -R | ./treestats

Output:

    n 1023, height 21

## Explanation

Increasing order creates a long tree with height 1023,
so a search may need 1023 visits.

Shuffled order creates a shorter tree with height 21,
so a search needs at most 21 visits for this tree.
