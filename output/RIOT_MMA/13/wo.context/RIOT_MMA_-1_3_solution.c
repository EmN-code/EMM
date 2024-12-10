#include <xtimer.h>
#include <stdio.h>
#include "mma8x5x.h"

#define CONFIG_MMA8X5X_I2C_ADDRESS (0x1D) // Default I2C address for MMA8x5x accelerometer
#define SLEEP_INTERVAL (5U) // Sleep interval in seconds

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8X5X\n");
        return 1;
    }

    while (1) {
        if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
            printf("Acceleration: X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        } else {
            printf("Error reading data\n");
        }
        xtimer_sleep(SLEEP_INTERVAL);
    }

    return 0;
}