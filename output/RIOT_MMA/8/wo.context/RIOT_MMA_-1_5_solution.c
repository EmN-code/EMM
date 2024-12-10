#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xtimer.h>
#include "periph/i2c.h"
#include "mma8x5x.h"

#define CONFIG_MMA8X5X_I2C_ADDRESS 0x1D  // Default I2C address for MMA8x5x
#define I2C_DEV     I2C_DEV(0)
#define INTERVAL    5  // Interval in seconds

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    printf("Initializing MMA8x5x accelerometer...\n");

    mma8x5x_params_t params = {
        .i2c = I2C_DEV,
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    int res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x: %d\n", res);
        return 1;
    }
    printf("MMA8x5x initialized successfully!\n");

    while (1) {
        res = mma8x5x_read(&dev, &data);
        if (res == MMA8X5X_OK) {
            printf("Acceleration - X: %d mg, Y: %d mg, Z: %d mg\n",
                   data.x, data.y, data.z);
        }
        else {
            printf("Error reading from MMA8x5x: %d\n", res);
        }

        xtimer_sleep(INTERVAL);
    }
    return 0;
}