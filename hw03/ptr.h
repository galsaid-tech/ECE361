#ifndef PTR_H
#define PTR_H

/* Swaps the values of two floats.
   Assumes both pointers are valid. */
void swap_float(float *a, float *b);

/* Finds the smallest and largest values in an array.
   Returns 0 on success and -1 for invalid input.
   Does not change outputs if input is invalid. */
int minmax(const float *a, int n, float *min, float *max);

/* Finds the first value greater than threshold.
   Returns its address, or NULL if not found.
   Assumes n is the number of array elements. */
const float *find_above(const float *a, int n, float threshold);

/* Reverses an array using two pointers.
   Does nothing if a is NULL or n is less than 2. */
void reverse(float *a, int n);

#endif