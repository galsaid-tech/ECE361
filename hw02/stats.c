#include "stats.h"

/* Find the smallest value in an array. */
float stats_min(const float a[], int n)
{
    float min = a[0];

    for (int i = 1; i < n; i++) {
        if (a[i] < min)
            min = a[i];
    }

    return min;
}

/* Find the largest value using recursion. */
float stats_max(const float a[], int n)
{
    if (n == 1)
        return a[0];

    float rest = stats_max(a + 1, n - 1);

    return a[0] > rest ? a[0] : rest;
}

/* Calculate the average of an array. */
float stats_mean(const float a[], int n)
{
    float sum = 0.0f;

    for (int i = 0; i < n; i++) {
        sum += a[i];
    }

    return sum / n;
}

/* Find the longest run above the threshold. */
int stats_longest_run_above(const float a[], int n,
                            float threshold, int *best_start)
{
    int run = 0;
    int best = 0;
    int start = 0;

    *best_start = -1;

    for (int i = 0; i < n; i++) {
        if (a[i] > threshold) {
            if (run == 0)
                start = i;

            run++;

            if (run > best) {
                best = run;
                *best_start = start;
            }
        } else {
            run = 0;
        }
    }

    return best;
}