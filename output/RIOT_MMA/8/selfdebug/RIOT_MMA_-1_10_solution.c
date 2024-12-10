#include <xtimer.h>
#include <stdio.h>
#include <periph/i2c.h>

#include "mma8x5x.h"

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = 0x1D, // Default I2C address for MMA8653
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

int main(void) {
    printf("Initializing MMA8x5x sensor...\n");
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x\n");
        return 1;
    }
    printf("MMA8x5x initialized successfully.\n");

    mma8x5x_data_t data;
    while (1) {
        if (mma8x5x_read(&dev, &data) != MMA8X5X_OK) {
            printf("Error reading data from MMA8x5x\n");
        } else {
            printf("Accelerometer data: X=%d, Y=%d, Z=%d\n", data.x, data.y, data.z);
        }
        xtimer_sleep(5); // delay 5 seconds
    }
    return 0;
}