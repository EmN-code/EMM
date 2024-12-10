#include "xtimer.h"
#include "periph/i2c.h"
#include "mma8x5x.h"
#include <stdio.h>

#define I2C_DEV      I2C_DEV(0)
#define ADDR         (0x1D) // The default I2C address for MMA8x5x

int main(void) {
    mma8x5x_t dev;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV,
        .addr = ADDR,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Initialization failed.");
        return 1;
    }

    while (1) {
        mma8x5x_data_t data;

        if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
            printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        } else {
            puts("Failed to read data");
        }

        xtimer_sleep(5); // Sleep for 5 seconds
    }

    return 0;
}