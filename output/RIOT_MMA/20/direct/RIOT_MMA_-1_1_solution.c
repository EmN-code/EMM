#include <stdio.h>
#include "xtimer.h"
#include "mma8x5x.h"
#include "periph/i2c.h"

#define SLEEP (5U * 1000000U) // Sleep for 5 seconds (in microseconds)
#define I2C DEV_I2C
#define ADDR ADDR_MMA8X5X

static mma8x5x_t dev;

int main(void) {
    int16_t x, y, z;
    mma8x5x_params_t mma_params = {
        .i2c_dev = I2C,
        .i2c_addr = ADDR,
    };

    printf("Initializing MMA8x5x accelerometer...\n");

    if (mma8x5x_init(&dev, &mma_params) != 0) {
        printf("Failed to initialize the MMA8x5x accelerometer\n");
        return 1;
    }

    printf("MMA8x5x accelerometer initialized successfully\n");

    while (1) {
        if (mma8x5x_read(&dev, &x, &y, &z) != 0) {
            printf("Failed to read data from the accelerometer\n");
        } else {
            printf("Acceleration - X: %d, Y: %d, Z: %d\n", x, y, z);
        }

        xtimer_usleep(SLEEP);
    }
    return 0;
}