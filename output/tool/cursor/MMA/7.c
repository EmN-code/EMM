#include <stdio.h>
#include "xtimer.h"
#include "periph/i2c.h"
#include "mma8x5x.h"

// Define the I2C interface and address for the MMA8x5x
#define I2C_DEV         I2C_DEV(0)
#define MMA8X5X_ADDR    0x1D

// Define the timer interval (5 seconds)
#define TIMER_INTERVAL  (5U * US_PER_SEC)

// Function to initialize the MMA8x5x sensor
static int init_mma8x5x(mma8x5x_t *dev) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV,
        .addr = MMA8X5X_ADDR,
        .rate = 0,  // Set appropriate rate
        .range = 0  // Set appropriate range
    };
    return mma8x5x_init(dev, &params);
}

// Function to read data from the MMA8x5x sensor
static void read_mma8x5x(mma8x5x_t *dev) {
    mma8x5x_data_t data;
    if (mma8x5x_read(dev, &data) == MMA8X5X_OK) {
        printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
    } else {
        printf("Failed to read data from MMA8x5x\n");
    }
}

int main(void) {
    mma8x5x_t dev;
    if (init_mma8x5x(&dev) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x\n");
        return 1;
    }

    // Main loop
    while (1) {
        read_mma8x5x(&dev);
        xtimer_sleep(TIMER_INTERVAL);
    }

    return 0;
}