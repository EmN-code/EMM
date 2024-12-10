#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <periph/i2c.h>
#include "mma8x5x.h"
#include "xtimer.h"

#define SLEEP_INTERVAL (5U * US_PER_SEC)

static mma8x5x_t dev;

int main(void) {
    /* Initialize parameters for MMA8x5x */
    mma8x5x_params_t params;
    params.i2c = I2C_DEV(0);
    params.addr = MMA8X5X_I2C_ADDRESS;
    params.rate = MMA8X5X_RATE_50HZ;
    params.range = MMA8X5X_RANGE_2G;

    /* Initialize the MMA8x5x sensor */
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Error initializing MMA8x5x\n");
        return 1;
    }

    while (1) {
        mma8x5x_data_t data;
        /* Read sensor data */
        if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
            printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        } else {
            printf("Error reading data\n");
        }
        /* Sleep for 5 seconds */
        xtimer_sleep(SLEEP_INTERVAL / US_PER_SEC);
    }
    return 0;
}