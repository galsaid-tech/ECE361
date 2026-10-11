#ifndef STATS_H
#define STATS_H

/*
 * Finds the smallest value in an array.
 * Assumes n is at least 1.
 * Returns the minimum value.
 */
float stats_min(const float a[], int n);

/*
 * Finds the largest value using recursion.
 * Assumes n is at least 1.
 * Returns the maximum value.
 */
float stats_max(const float a[], int n);

/*
 * Calculates the average of an array.
 * Assumes n is at least 1.
 * Returns the mean value.
 */
float stats_mean(const float a[], int n);

/*
 * Finds the longest group of consecutive values
 * greater than the threshold.
 * Assumes n is at least 0 and best_start is valid.
 * Returns the length and stores the starting index.
 * Stores -1 if no values are above the threshold.
 */
int stats_longest_run_above(const float a[], int n,
                            float threshold, int *best_start);

#endif