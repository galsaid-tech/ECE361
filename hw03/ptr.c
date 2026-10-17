
#include "ptr.h"
#include <stddef.h>

// Function 1: Swap two float numbers
void swap_float(float *a, float *b)
{
    float temp = *a;
    *a = *b;
    *b = temp;
}

// Function 2: Find the smallest and largest numbers
int minmax(const float *a, int n, float *min, float *max)
{
    if (a == NULL || min == NULL || max == NULL || n <= 0)
    {
        return -1;
    }

    float smallest = *a;
    float largest = *a;

    const float *p = a + 1;

    while (p < a + n)
    {
        if (*p < smallest)
        {
            smallest = *p;
        }

        if (*p > largest)
        {
            largest = *p;
        }

        p++;
    }

    *min = smallest;
    *max = largest;

    return 0;
}

// Function 3: Find the first number above the threshold
const float *find_above(const float *a, int n, float threshold)
{
    if (a == NULL || n <= 0)
    {
        return NULL;
    }

    const float *p = a;

    while (p < a + n)
    {
        if (*p > threshold)
        {
            return p;
        }

        p++;
    }

    return NULL;
}

// Function 4: Reverse the array
void reverse(float *a, int n)
{
    if (a == NULL || n < 2)
    {
        return;
    }

    float *left = a;
    float *right = a + n - 1;

    while (left < right)
    {
        swap_float(left, right);

        left++;
        right--;
    }
}
