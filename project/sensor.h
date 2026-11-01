
#ifndef SENSOR_H
#define SENSOR_H

#include <time.h>

/*
 * This module will read temperature and humidity
 * from the iom361 sensor.
 *
 * It will convert the sensor data into values
 * that the wine cellar monitor can use.
 */

/* Stores one sensor reading */
typedef struct {
    time_t timestamp;
    float temperature;
    float humidity;
} reading_t;
#include <stdint.h>

int sensor_read(uint32_t *base, time_t timestamp,
                reading_t *reading);
#endif
