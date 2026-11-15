
#include "istack.h"

#include <stdio.h>

static int is_open(int c)
{
    return c == '(' || c == '[' || c == '{';
}

static int is_close(int c)
{
    return c == ')' || c == ']' || c == '}';
}

static int matching_close(int open)
{
    if (open == '(') return ')';
    if (open == '[') return ']';
    return '}';
}

int main(void)
{
    istack_t *s = istack_create();
    if (s == NULL) {
        fputs("error: out of memory\n", stderr);
        return 1;
    }

    size_t line = 1;
    size_t column = 0;
    size_t pairs = 0;
    size_t deepest = 0;
    int c;
    int status = 0;

    while ((c = getchar()) != EOF) {
        if (c == '\n') {
            line++;
            column = 0;
            continue;
        }

        column++;

        if (is_open(c)) {
            if (istack_push(s, c) != 0) {
                fputs("error: out of memory\n", stderr);
                status = 1;
                goto done;
            }

            if (istack_count(s) > deepest) {
                deepest = istack_count(s);
            }
        }
        else if (is_close(c)) {
            int top;

            if (istack_peek(s, &top) != 0) {
                printf("line %zu, column %zu: found '%c', nothing open\n",
                       line, column, c);
                status = 1;
                goto done;
            }

            if (matching_close(top) != c) {
                printf("line %zu, column %zu: found '%c', expected '%c'\n",
                       line, column, c, matching_close(top));
                status = 1;
                goto done;
            }

            istack_pop(s, NULL);
            pairs++;
        }
    }

    if (!istack_is_empty(s)) {
        int top;
        istack_peek(s, &top);

        printf("end of input: %zu still open, innermost '%c'\n",
               istack_count(s), (char)top);
        status = 1;
    }
    else {
        printf("balanced: %zu pairs, deepest %zu\n",
               pairs, deepest);
    }

done:
    // istack_destroy(s);  // Bug 3: Memory leak
    return status;
}
