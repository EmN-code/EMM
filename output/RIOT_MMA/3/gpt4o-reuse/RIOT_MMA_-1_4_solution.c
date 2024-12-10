
#include <stdio.h>
#include "periph/i2c.h"
#include "mma8x5x.h"
#include "xtimer.h"

#define MMA8X5X_I2C_ADDRESS 0x1D
#define MMA8X5X_RATE_50HZ MMA8X5X_RATE_50HZ
#define MMA8X5X_RANGE_2G MMA8X5X_RANGE_2G

static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

static mma8x5x_t dev;

int main(void) {
    int ret;
    mma8x5x_data_t data;

    // Initialize the MMA8x5x sensor
    ret = mma8x5x_init(&dev, &params);
    if (ret != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x sensor\n");
        return 1;
    }

    while (1) {
        // Read the acceleration data
        mma8x5x_read(&dev, &data);

        // Print the acceleration data
        printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);

        // Delay for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}
