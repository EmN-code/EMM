#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "periph/i2c.h"
#include "mma8x5x.h"

#define I2C_INTERFACE I2C_DEV(0)
#define MMA8X5X_I2C_ADDRESS 0x1D  // This could be different, double-check your sensor's datasheet

int main(void) {
    mma8x5x_t dev;
    mma8x5x_params_t params = {
        .i2c = I2C_INTERFACE,
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x sensor\n");
        return -1;
    }

    while (1) {
        mma8x5x_data_t data;
        if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
            printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        } else {
            printf("Failed to read data\n");
        }
        usleep(5000000);  // Sleep for 5 seconds
    }

    return 0;
}