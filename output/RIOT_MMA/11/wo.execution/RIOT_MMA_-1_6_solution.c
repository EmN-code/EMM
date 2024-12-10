#include "xtimer.h"
#include "ztimer.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    printf("Initializing MMA8x5x accelerometer...\n");

    // Initialize the accelerometer with revised address constant
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS, // Corrected constant name
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    // Check if initialization is successful
    int init_status = mma8x5x_init(&dev, &params);
    if (init_status != MMA8X5X_OK) {
        printf("Failed to initialize accelerometer\n");
        return -1;
    }

    // Periodically read data
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 5000);
        mma8x5x_read(&dev, &data);
        printf("Acceleration Data: X = %d mg, Y = %d mg, Z = %d mg\n",
               data.x, data.y, data.z);
    }

    return 0;
}