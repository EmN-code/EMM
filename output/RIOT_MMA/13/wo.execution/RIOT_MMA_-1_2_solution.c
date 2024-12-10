#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "xtimer.h"
#include <stdio.h>

#define SLEEP_TIME (5U * US_PER_SEC)

int main(void) {
    /* Device descriptor */
    mma8x5x_t dev;
    mma8x5x_data_t data;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    /* Initialize the device */
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Initialization failed");
        return 1;
    }

    /* Main loop */
    while (1) {
        /* Read accelerometer data */
        mma8x5x_read(&dev, &data);

        printf("Acceleration (mg): X = %d, Y = %d, Z = %d\n", data.x_axis, data.y_axis, data.z_axis);

        /* Wait for the next cycle */
        xtimer_sleep(SLEEP_TIME);
    }

    return 0;
}