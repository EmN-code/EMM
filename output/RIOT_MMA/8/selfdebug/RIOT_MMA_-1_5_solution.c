#include <stdio.h>
#include "periph/i2c.h"
#include "mma8x5x.h"
#include "xtimer.h"

#define I2C_ADDR        (0x1D)  /* Define your MMA8x5x I2C address here */
#define SLEEP_INTERVAL  (5U * 1000000U) /* 5 seconds in microseconds */

static mma8x5x_t dev;

int main(void) {
    
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = I2C_ADDR,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };
    
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Failed to initialize MMA8x5x");
        return 1;
    }

    while (1) {
        mma8x5x_data_t data;
        mma8x5x_read(&dev, &data); /* Read sensor data */

        printf("X: %d mg, Y: %d mg, Z: %d mg\n", data.x_axis, data.y_axis, data.z_axis);

        xtimer_sleep(SLEEP_INTERVAL / 1000000U);  /* Sleep for 5 seconds */
    }

    return 0;
}