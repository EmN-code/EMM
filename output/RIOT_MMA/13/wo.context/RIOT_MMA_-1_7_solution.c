#include <stdio.h>
#include <periph/i2c.h>
#include "mma8x5x.h"

static mma8x5x_t dev;
static mma8x5x_data_t data;

void mma8x5x_read_data(void) {
    if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
        printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
    } else {
        printf("Error reading data\n");
    }
}

int main(void) {
    printf("MMA8x5x Driver Test\n");

    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS, // Correct address
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Initialization failed\n");
        return -1;
    }

    while (1) {
        mma8x5x_read_data();
        xtimer_sleep(5); // sleep for 5 seconds
    }

    return 0;
}