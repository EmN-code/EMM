#include <stdio.h>            // for standard input/output functions
#include <stdint.h>           // for standard integer types
#include "xtimer.h"          // for timing functions
#include "periph/i2c.h"      // for I2C peripheral functions
#include "mma8x5x.h"         // mma8x5x sensor driver

static mma8x5x_t dev;

// Function to initialize the accelerometer
static int accel_init(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) == MMA8X5X_OK) {
        puts("MMA8x5x initialized successfully.");
        return 0;
    } else {
        puts("Failed to initialize MMA8x5x.");
        return -1;
    }
}

// Function to read and print accelerometer data
static void read_sensor_data(void) {
    mma8x5x_data_t data;

    if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
        printf("Acceleration X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
    } else {
        puts("Error reading from MMA8x5x.");
    }
}

int main(void) {
    puts("RIOT MMA8x5x accelerometer example.");

    if (accel_init() != 0) {
        puts("Error: initialization failed.");
        return 1;
    }

    while (1) {
        read_sensor_data();
        xtimer_sleep(5);
    }

    return 0;
}