#include <stdio.h>
#include <stdlib.h>
#include "xtimer.h"
#include "mma8x5x.h"

static mma8x5x_t mma_device;
static mma8x5x_data_t mma_data;

void read_mma8x5x(void) {
    mma8x5x_read(&mma_device, &mma_data);
    printf("Acceleration [mg]: X=%d Y=%d Z=%d\n", mma_data.x, mma_data.y, mma_data.z);
}

int main(void) {
    printf("MMA8x5x accelerometer test\n");

    /* Initialize the MMA8x5x sensor */
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&mma_device, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x\n");
        return -1;
    }

    /* Periodically read from the mma8x5x every 5 seconds */
    while (1) {
        read_mma8x5x();
        xtimer_sleep(5);
    }

    return 0;
}