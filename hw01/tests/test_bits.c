#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <unistd.h>

#include "../bits.h"
#include "../status.h"

static int tests_run = 0;
static int tests_passed = 0;

void check_int(const char *name, int32_t actual, int32_t expected)
{
    tests_run++;

    if (actual == expected)
    {
        printf("PASS: %s\n", name);
        tests_passed++;
    }
    else
    {
        printf("FAIL: %s -- expected %d, got %d\n",
               name, expected, actual);
    }
}

void check_uint(const char *name, uint32_t actual, uint32_t expected)
{
    tests_run++;

    if (actual == expected)
    {
        printf("PASS: %s\n", name);
        tests_passed++;
    }
    else
    {
        printf("FAIL: %s -- expected %u, got %u\n",
               name, expected, actual);
    }
}

void check_print_binary(const char *name,
                        uint32_t x,
                        int width,
                        const char *expected)
{
    int pipefd[2];
    int saved_stdout;
    char buffer[128] = {0};

    tests_run++;

    saved_stdout = dup(STDOUT_FILENO);

    if (pipe(pipefd) != 0 || saved_stdout < 0)
    {
        printf("FAIL: %s\n", name);
        return;
    }

    fflush(stdout);

    dup2(pipefd[1], STDOUT_FILENO);
    close(pipefd[1]);

    print_binary(x, width);

    fflush(stdout);

    dup2(saved_stdout, STDOUT_FILENO);
    close(saved_stdout);

    ssize_t count =
        read(pipefd[0], buffer, sizeof(buffer) - 1);

    close(pipefd[0]);

    if (count < 0)
    {
        printf("FAIL: %s\n", name);
        return;
    }

    buffer[count] = '\0';

    if (strcmp(buffer, expected) == 0)
    {
        printf("PASS: %s\n", name);
        tests_passed++;
    }
    else
    {
        printf("FAIL: %s\n", name);
        printf("Expected: %s", expected);
        printf("Got:      %s", buffer);
    }
}

int main(void)
{
    printf("Testing print_binary\n");

    check_print_binary(
        "print_binary example",
        0x2C,
        8,
        "0010 1100\n"
    );

    check_print_binary(
        "print_binary width 1",
        1,
        1,
        "1\n"
    );

    printf("\nTesting get_field\n");

    check_uint(
        "get_field normal",
        get_field(0x2C, 2, 3),
        3
    );

    check_uint(
        "get_field width 1",
        get_field(1, 0, 1),
        1
    );

    check_uint(
        "get_field pos 31",
        get_field(0x80000000U, 31, 1),
        1
    );

    check_uint(
        "get_field width 32",
        get_field(0x12345678U, 0, 32),
        0x12345678U
    );

    check_uint(
        "get_field invalid range",
        get_field(0, 31, 2),
        0
    );

    printf("\nTesting set_field\n");

    check_uint(
        "set_field normal",
        set_field(0, 4, 3, 5),
        0x50
    );

    check_uint(
        "set_field value too wide",
        set_field(0, 0, 3, 15),
        7
    );

    check_uint(
        "set_field width 32",
        set_field(0, 0, 32, 0xABCDEF12U),
        0xABCDEF12U
    );

    check_uint(
        "set_field invalid range",
        set_field(0x12345678U, 31, 2, 3),
        0x12345678U
    );

    printf("\nTesting sign_extend\n");

    check_int(
        "sign_extend -8",
        sign_extend(0xF8, 8),
        -8
    );

    check_int(
        "sign_extend positive",
        sign_extend(0x7F, 8),
        127
    );

    check_int(
        "sign_extend most negative 8-bit",
        sign_extend(0x80, 8),
        -128
    );

    check_int(
        "sign_extend width 1",
        sign_extend(1, 1),
        -1
    );

    printf("\nTesting status_unpack\n");

    status_t s1 = status_unpack(0x1631);

    check_int(
        "status example setpoint",
        s1.setpoint,
        22
    );

    check_int(
        "status example mode",
        s1.mode,
        3
    );

    check_int(
        "status example heat",
        s1.heat,
        1
    );

    check_int(
        "status example cool",
        s1.cool,
        0
    );

    check_int(
        "status example fan",
        s1.fan,
        0
    );

    check_int(
        "status example fault",
        s1.fault,
        0
    );

    check_int(
        "status example reserved",
        s1.reserved,
        0
    );

    status_t s2 = status_unpack(0xFF00);

    check_int(
        "negative setpoint",
        s2.setpoint,
        -1
    );

    status_t s3 = status_unpack(0x0050);

    check_int(
        "invalid mode",
        s3.mode,
        5
    );

    printf("\nSummary: %d/%d tests passed\n",
           tests_passed, tests_run);

    if (tests_passed == tests_run)
    {
        return 0;
    }

    return 1;
}
