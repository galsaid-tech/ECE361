#include "../istack.h"

#include <stdio.h>
#include <stdlib.h>

static int passed = 0;
static int failed = 0;

static void result(const char *name, int ok)
{
    printf("%s: %s\n", ok ? "PASS" : "FAIL", name);
    if (ok) passed++; else failed++;
}

static void test_empty(void)
{
    istack_t *s = istack_create();
    int value = 123;
    int ok = s != NULL;
    if (ok) {
        ok = istack_is_empty(s) && istack_count(s) == 0 &&
             istack_capacity(s) == ISTACK_START &&
             istack_pop(s, &value) == -1 && value == 123 &&
             istack_peek(s, &value) == -1 && value == 123;
    }
    result("empty stack and unchanged output", ok);
    istack_destroy(s);
}

static void test_lifo(void)
{
    istack_t *s = istack_create();
    int value = 0;
    int ok = s != NULL;
    if (ok) {
        ok = istack_push(s, 11) == 0 && istack_push(s, 22) == 0 &&
             istack_push(s, 33) == 0 && istack_peek(s, &value) == 0 &&
             value == 33 && istack_count(s) == 3;
    }
    if (ok) ok = istack_pop(s, &value) == 0 && value == 33;
    if (ok) ok = istack_pop(s, &value) == 0 && value == 22;
    if (ok) ok = istack_pop(s, &value) == 0 && value == 11;
    if (ok) ok = istack_is_empty(s);
    result("last in, first out", ok);
    istack_destroy(s);
}

static void test_growth(void)
{
    istack_t *s = istack_create();
    int ok = s != NULL;
    int value = -1;
    if (ok) {
        for (int i = 0; i < 8; i++) {
            if (istack_push(s, i) != 0) ok = 0;
        }
    }
    if (ok) ok = istack_capacity(s) == 8 && istack_count(s) == 8;
    if (ok) ok = istack_push(s, 8) == 0 && istack_capacity(s) == 16;
    if (ok) {
        for (int i = 8; i >= 0; i--) {
            if (istack_pop(s, &value) != 0 || value != i) ok = 0;
        }
    }
    result("8 to 16 growth and kept values", ok);
    istack_destroy(s);
}

static void test_1000(void)
{
    istack_t *s = istack_create();
    int ok = s != NULL;
    int value = -1;
    if (ok) {
        for (int i = 0; i < 1000; i++) {
            if (istack_push(s, i) != 0) ok = 0;
        }
    }
    if (ok) ok = istack_count(s) == 1000 && istack_capacity(s) == 1024;
    if (ok) {
        for (int i = 999; i >= 0; i--) {
            if (istack_pop(s, &value) != 0 || value != i) ok = 0;
        }
    }
    result("1000 pushes and pops", ok);
    istack_destroy(s);
}

static void test_array(void)
{
    istack_t *s = istack_create();
    size_t n = 77;
    int ok = s != NULL;
    int *a = NULL;
    int value = 0;
    if (ok) ok = istack_to_array(s, &n) == NULL && n == 0;
    if (ok) ok = istack_push(s, 5) == 0 && istack_push(s, 6) == 0 &&
                 istack_push(s, 7) == 0;
    if (ok) {
        a = istack_to_array(s, &n);
        ok = a != NULL && n == 3 && a[0] == 5 && a[1] == 6 && a[2] == 7;
    }
    if (ok) {
        a[2] = 100;
        ok = istack_peek(s, &value) == 0 && value == 7;
    }
    result("to_array: bottom first and independent copy", ok);
    free(a);
    istack_destroy(s);
}

static void test_null(void)
{
    int value = 91;
    size_t n = 19;
    int ok = istack_push(NULL, 1) == -1 &&
             istack_pop(NULL, &value) == -1 && value == 91 &&
             istack_peek(NULL, &value) == -1 && value == 91 &&
             istack_count(NULL) == 0 && istack_is_empty(NULL) &&
             istack_capacity(NULL) == 0 &&
             istack_to_array(NULL, &n) == NULL && n == 0;
    istack_destroy(NULL);
    result("NULL arguments", ok);
}

static void test_null_outputs(void)
{
    istack_t *s = istack_create();
    int ok = s != NULL;
    int *copy = NULL;
    if (ok) ok = istack_push(s, 42) == 0 &&
                 istack_peek(s, NULL) == 0 &&
                 istack_pop(s, NULL) == 0 &&
                 istack_pop(s, NULL) == -1 &&
                 istack_push(s, 19) == 0;
    if (ok) {
        copy = istack_to_array(s, NULL);
        ok = copy != NULL && copy[0] == 19;
    }
    result("optional NULL output pointers", ok);
    free(copy);
    istack_destroy(s);
}

static void test_two_stacks(void)
{
    istack_t *a = istack_create();
    istack_t *b = istack_create();
    int value = 0;
    int ok = a != NULL && b != NULL;
    if (ok) ok = istack_push(a, 10) == 0 && istack_push(b, 20) == 0 &&
                 istack_push(a, 30) == 0 && istack_count(a) == 2 &&
                 istack_count(b) == 1 && istack_peek(b, &value) == 0 &&
                 value == 20;
    result("two independent stacks", ok);
    istack_destroy(a);
    istack_destroy(b);
}

static void test_destroy_not_empty(void)
{
    istack_t *s = istack_create();
    int ok = s != NULL;
    if (ok) {
        for (int i = 0; i < 20; i++) {
            if (istack_push(s, i) != 0) ok = 0;
        }
    }
    /* Sanitizers or leaks tools check that freeing this nonempty stack is safe. */
    istack_destroy(s);
    result("destroy a nonempty stack", ok);
}

int main(void)
{
    test_empty();
    test_lifo();
    test_growth();
    test_1000();
    test_array();
    test_null();
    test_null_outputs();
    test_two_stacks();
    test_destroy_not_empty();

    printf("Summary: %d passed, %d failed\n", passed, failed);
    return failed == 0 ? 0 : 1;
}