#include <xtimer.h>
#include <stdio.h>
#include <periph/i2c.h>
#include "mma8x5x.h"

// Define I2C Address for MMA8x5x sensor (this should match your setup)
#ifndef CONFIG_MMA8X5X_I2C_ADDRESS
#define CONFIG_MMA8X5X_I2C_ADDRESS 0x1C
#endif

// Define timer interval in microseconds
#define INTERVAL (5U * US_PER_SEC)

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Error: Unable to initialize sensor\n");
        return 1;
    }

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("X: %d, Y: %d, Z: %d\n", data.x_axis, data.y_axis, data.z_axis);
        xtimer_usleep(INTERVAL);
    }
    return 0;
}