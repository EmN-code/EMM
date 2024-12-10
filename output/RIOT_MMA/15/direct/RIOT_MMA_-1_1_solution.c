#include <stdio.h>
#include "xtimer.h"
#include "mma8x5x.h"

#define INTERVAL (5U * US_PER_SEC) // interval of 5 seconds

int main(void) {
    mma8x5x_t dev;
    mma8x5x_data_t data;

    // Initialize MMA8x5x sensor
    if (mma8x5x_init(&dev, I2C_BUS)) {
        puts("MMA8x5x initialization failed!");
        return 1;
    }

    while (1) {
        // Read data from the sensor
        if (mma8x5x_read(&dev, &data) == 0) {
            printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        }
        else {
            puts("Failed to read data from MMA8x5x sensor");
        }

        // Wait for the specified interval
        xtimer_sleep(INTERVAL / US_PER_SEC);
    }

    return 0;
}