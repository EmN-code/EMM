#include <stdio.h>
#include <stdlib.h>
#include "periph/i2c.h"
#include "mma8x5x.h"
#include "xtimer.h"

static mma8x5x_t dev;

int main(void) {
    // Initialize parameters for the MMA8x5x device
    mma8x5x_params_t params;
    params.i2c = I2C_DEV(0);
    params.addr = MMA8X5X_I2C_ADDRESS;
    params.rate = MMA8X5X_RATE_50HZ;
    params.range = MMA8X5X_RANGE_2G;

    // Initialize the MMA8x5x device
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x\n");
        return -1;
    }

    printf("MMA8x5x initialized\n");

    while (1) {
        xtimer_sleep(5);  // Sleep for 5 seconds

        mma8x5x_data_t data;

        // Read data from the MMA8x5x sensor
        if (mma8x5x_read(&dev, &data) != MMA8X5X_OK) {
            printf("Failed to read data from MMA8x5x\n");
            continue;
        }

        // Print the read accelerometer data
        printf("Accelerometer data: X=%d, Y=%d, Z=%d\n", data.x, data.y, data.z);
    }

    return 0; // Although not reached
}