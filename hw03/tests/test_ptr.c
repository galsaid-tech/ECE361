
#include <stdio.h>
#include "ptr.h"

int main(void)
{
    int passed = 0;

    // Test 1: swap_float
    float x = 5.0f;
    float y = 9.0f;

    swap_float(&x, &y);

    if (x == 9.0f && y == 5.0f)
    {
        printf("PASS swap_float\n");
        passed++;
    }
    else
    {
        printf("FAIL swap_float\n");
    }

    // Test 2: minmax with several numbers
    float a[] = {5.0f, 2.0f, 9.0f, 4.0f};
    float min = 0.0f;
    float max = 0.0f;

    int result = minmax(a, 4, &min, &max);

    if (result == 0 && min == 2.0f && max == 9.0f)
    {
        printf("PASS minmax\n");
        passed++;
    }
    else
    {
        printf("FAIL minmax\n");
    }

    // Test 3: find_above
    float b[] = {3.0f, 7.0f, 5.0f};
    const float *found = find_above(b, 3, 6.0f);

    if (found == &b[1])
    {
        printf("PASS find_above\n");
        passed++;
    }
    else
    {
        printf("FAIL find_above\n");
    }

    // Test 4: reverse even array
    float c[] = {2.0f, 4.0f, 6.0f, 8.0f};

    reverse(c, 4);

    if (c[0] == 8.0f && c[1] == 6.0f &&
        c[2] == 4.0f && c[3] == 2.0f)
    {
        printf("PASS reverse even\n");
        passed++;
    }
    else
    {
        printf("FAIL reverse even\n");
    }

    // Test 5: minmax NULL array
    float low = 100.0f;
    float high = 200.0f;

    int error = minmax(NULL, 0, &low, &high);

    if (error == -1 && low == 100.0f && high == 200.0f)
    {
        printf("PASS minmax NULL array\n");
        passed++;
    }
    else
    {
        printf("FAIL minmax NULL array\n");
    }

    // Test 6: find_above no match
    float values[] = {2.0f, 4.0f, 6.0f};

    const float *answer = find_above(values, 3, 10.0f);

    if (answer == NULL)
    {
        printf("PASS find_above no match\n");
        passed++;
    }
    else
    {
        printf("FAIL find_above no match\n");
    }

    // Test 7: reverse one number
    float single[] = {5.0f};

    reverse(single, 1);

    if (single[0] == 5.0f)
    {
        printf("PASS reverse one number\n");
        passed++;
    }
    else
    {
        printf("FAIL reverse one number\n");
    }

    // Test 8: minmax one number
    float one[] = {7.0f};
    float one_min = 0.0f;
    float one_max = 0.0f;

    int one_result = minmax(one, 1, &one_min, &one_max);

    if (one_result == 0 && one_min == 7.0f &&
        one_max == 7.0f)
    {
        printf("PASS minmax one number\n");
        passed++;
    }
    else
    {
        printf("FAIL minmax one number\n");
    }

    // Test 9: minmax with n = 0
    float empty_test[] = {5.0f};
    float empty_min = 10.0f;
    float empty_max = 20.0f;

    int empty_result = minmax(empty_test, 0,
                              &empty_min, &empty_max);

    if (empty_result == -1 && empty_min == 10.0f &&
        empty_max == 20.0f)
    {
        printf("PASS minmax n zero\n");
        passed++;
    }
    else
    {
        printf("FAIL minmax n zero\n");
    }

    // Test 10: minmax NULL output
    float test_values[] = {2.0f, 8.0f};
    float test_max = 50.0f;

    int null_result = minmax(test_values, 2, NULL, &test_max);

    if (null_result == -1 && test_max == 50.0f)
    {
        printf("PASS minmax NULL output\n");
        passed++;
    }
    else
    {
        printf("FAIL minmax NULL output\n");
    }

    // Test 11: find_above two matches
    float two[] = {3.0f, 8.0f, 9.0f};

    const float *first = find_above(two, 3, 6.0f);

    if (first == &two[1])
    {
        printf("PASS find_above first match\n");
        passed++;
    }
    else
    {
        printf("FAIL find_above first match\n");
    }

    // Test 12: find_above equal threshold
    float equal[] = {5.0f, 7.0f};

    const float *equal_result = find_above(equal, 2, 5.0f);

    if (equal_result == &equal[1])
    {
        printf("PASS find_above equal threshold\n");
        passed++;
    }
    else
    {
        printf("FAIL find_above equal threshold\n");
    }

    // Test 13: reverse odd array
    float odd[] = {1.0f, 2.0f, 3.0f, 4.0f, 5.0f};

    reverse(odd, 5);

    if (odd[0] == 5.0f && odd[1] == 4.0f &&
        odd[2] == 3.0f && odd[3] == 2.0f &&
        odd[4] == 1.0f)
    {
        printf("PASS reverse odd\n");
        passed++;
    }
    else
    {
        printf("FAIL reverse odd\n");
    }

    printf("Passed %d out of 13 tests\n", passed);

    return passed == 13 ? 0 : 1;
}
