
#include <stdio.h>
#include "periph/i2c.h"
#include "mma8x5x.h"
#include "xtimer.h"

#define MMA8X5X_I2C_ADDRESS 0x1D
#define MMA8X5X_RATE_50HZ MMA8X5X_RATE_50HZ
#define MMA8X5X_RANGE_2G MMA8X5X_RANGE_2G

static mma8x5x_t dev;
static mma8x5x_params_t params;

int main(void) {
    // Initialize the I2C bus
    i2c_init(I2C_DEV(0));

    // Set up the MMA8x5x parameters
    params.i2c = I2C_DEV(0);
    params.addr = MMA8X5X_I2C_ADDRESS;
    params.rate = MMA8X5X_RATE_50HZ;
    params.range = MMA8X5X_RANGE_2G;

    // Initialize the MMA8x5x accelerometer
    int res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x accelerometer\n");
        return 1;
    }

    mma8x5x_data_t data;
    while (1) {
        // Read the accelerometer data
        mma8x5x_read(&dev, &data);

        // Print the acceleration data
        printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);

        // Delay for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}
