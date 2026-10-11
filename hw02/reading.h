#ifndef READING_H
#define READING_H

#include <stdio.h>

#define MAX_READINGS 1000

/*
 * Reads sensor data from input.
 * Stores ticks, temperatures, and humidity.
 * Returns the number of valid readings.
 * Assumes arrays can hold MAX_READINGS values.
 */
int reading_read(FILE *input, int ticks[],
                 float temps[], float hums[],
                 int *skipped);

#endif