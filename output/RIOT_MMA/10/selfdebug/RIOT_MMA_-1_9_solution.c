#include "xtimer.h"
#include "mma8x5x.h"
#include "periph/i2c.h"
#include <stdio.h>

#define INTERVAL 5U * 1000000U // 5 seconds

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    // Initialize I2C and MMA8x5x parameters
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    // Initialize the accelerometer
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize accelerometer\n");
        return 1;
    }

    // Continuously read data every 5 seconds
    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration data: X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);
        xtimer_usleep(INTERVAL);
    }

    return 0;
}