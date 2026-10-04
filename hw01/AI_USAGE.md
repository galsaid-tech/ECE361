# AI Usage

I used ChatGPT to help me understand the homework requirements, explain bit manipulation, and create a first version of the C code and tests.

I used ChatGPT for:
- explaining get_field, set_field, and sign_extend
- helping write bits.h and bits.c
- helping write status.h and status.c
- helping create the Makefile
- helping create tests

One thing I had to check carefully was the case where width is 32.

Using:
1U << 32
is not valid for a 32-bit unsigned integer, so I handled width == 32 separately.
I checked my work by compiling with:
gcc -std=c11 -Wall -Wextra

and by running the tests with:

make test