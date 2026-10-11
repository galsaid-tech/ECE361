# AI Usage - Homework 2

I used ChatGPT to help me understand and complete Homework 2.

ChatGPT helped me split the original `readings.c` program into smaller files, including `reading.c`, `stats.c`, `histogram.c`, and `main.c`.

I also used ChatGPT to understand recursion, local variables, header files, and the Makefile. It explained the code using simple English and helped me understand how to build and test my program.

**One mistake AI made:**

ChatGPT first gave me code for `test_stats.c` that used global variables called `tests_run` and `tests_passed`.

I found the problem by checking my professor's instructions, which said not to use global variables. ChatGPT also pointed out the mistake. I fixed it by moving both variables inside `main()` and changing `check_test` to return a value.

After making the changes, I ran `make` and `make test`. All four statistics tests and all four regression tests passed.

I also ran `make clean` to check that the compiled files could be removed.
