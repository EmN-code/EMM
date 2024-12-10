#include "thread.h"
#include "time_units.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "event/thread.h"
#include <stdio.h>

#define SLEEP_INTERVAL (5LU * US_PER_SEC)

int main(void)
{
    // Device descriptor structure
    mma8x5x_t dev;

    // Initialize parameters for the MMA8x5x
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS,  // Corrected macro
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G  // Used correct range macro
    };

    // Initialize the device
    int res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("Initialization failed with code %d\n", res);
        return -1;
    }

    // Variable to store accelerometer data
    mma8x5x_data_t data;

    while (1) {
        // Fetch accelerometer data
        res = mma8x5x_read(&dev, &data);
        if (res == MMA8X5X_OK) {
            printf("X: %d Y: %d Z: %d\n", data.x, data.y, data.z);
        } else {
            printf("Error reading data: %d\n", res);
        }

        // Sleep for 5 seconds
        xtimer_usleep(SLEEP_INTERVAL);
    }

    return 0;
}