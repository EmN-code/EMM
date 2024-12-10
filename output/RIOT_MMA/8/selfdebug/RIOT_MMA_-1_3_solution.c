#include "periph/i2c.h"
#include "kernel_defines.h"
#include <stdio.h>

#define MMA8X5X_I2C_ADDRESS (0x1D)  // Define the I2C address if not provided

static void timer_callback(void *arg) {
    mma8x5x_data_t data;
    if (mma8x5x_read(&data) != MMA8X5X_OK) {
        printf("Failed to read data from MMA8x5x\n");
        return;
    }
    printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
}

int main(void) {
    mma8x5x_t dev;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x\n");
        return 1;
    }

    while (1) {
        timer_callback(NULL);
        xtimer_usleep(5000000);  // Sleep for 5 seconds
    }

    return 0;
}