#include <xtimer.h>
#include <stdio.h>
#include <mma8x5x.h>
#include <i2c.h>

int main(void) {
    /* Initialize variables */
    mma8x5x_t dev;
    mma8x5x_data_t data;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    /* Initialize accelerometer */
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x\n");
        return 1;
    }

    /* Loop to read acceleration data every 5 seconds */
    while (1) {
        mma8x5x_read(&dev, &data);
        printf("X: %d mg\tY: %d mg\tZ: %d mg\n", data.x_axis, data.y_axis, data.z_axis);

        /* Sleep for 5 seconds */
        xtimer_sleep(5);
    }

    return 0;
}