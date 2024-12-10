#include <ztimer.h>
#include <stdio.h>
#include "periph/i2c.h"
#include "mma8x5x.h"

#define I2C_ADDR        (0x1D)   // Default I2C address for MMA8X5X
#define I2C_DEV         I2C_DEV(0)
#define SAMPLE_RATE     (5000000U) // 5 seconds in microseconds

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    // Setup parameters for MMA8x5x
    mma8x5x_params_t params = {
        .i2c = I2C_DEV,
        .addr = I2C_ADDR,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };
    
    // Initialize the device
    int res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("Failed to initialize MMA8X5x\n");
        return 1;
    }

    // Infinite loop to read data
    while (1) {
        // Read data from the sensor
        mma8x5x_read(&dev, &data);

        // Output the acceleration data
        printf("X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);

        // Wait for the next sample
        ztimer_sleep(ZTIMER_MSEC, SAMPLE_RATE);
    }

    return 0;
}