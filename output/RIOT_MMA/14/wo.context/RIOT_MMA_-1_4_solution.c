#include "mma8x5x.h"
#include "xtimer.h"
#include <stdio.h>

/* MMA8X5X device descriptor */
static mma8x5x_t dev;

/* Read data from MMA8x5x accelerometer every 5 seconds */
int main(void) {
    /* MMA8X5X accelerometer parameters initialization */
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = 0x1D,  // correct the I2C address here
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    /* Initialize sensor */
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Error initializing MMA8x5x\n");
        return 1;
    }

    /* Main loop */
    while(1) {
        mma8x5x_data_t data;
        mma8x5x_read(&dev, &data);
        printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);

        /* wait 5 seconds */
        xtimer_sleep(5);
    }

    return 0;
}