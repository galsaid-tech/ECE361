# ECE 361 – Homework 9

## About This Homework

This homework is about hash tables in C.

I created a hash table using linked lists. Each key has a count that increases when the same key is added again.

## Functions

- **ht_create:** Creates an empty hash table.
- **ht_destroy:** Frees the memory.
- **ht_add:** Adds a key or increases its count.
- **ht_count:** Returns how many times a key was added.
- **ht_size:** Returns the number of different keys.
- **ht_longest_chain:** Finds the longest linked list.

## Build and Test

Run these commands inside the hw09 folder:

```bash
make
make test
make memcheck
make clean
```

## Longest Chain Explanation

With 1000 buckets, multiples of 10 share buckets because their remainders repeat, so the longest chain has 10 nodes instead of 1.

With 1009 buckets, the 1000 keys use different buckets because 1009 and 10 have no common factor, so the longest chain is 1.

## Test Results

All 11 tests passed.

I also used AddressSanitizer to check the program, and no errors were reported.