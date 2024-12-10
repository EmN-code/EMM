#include <xtimer.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <periph/gpio.h>
#include <periph/i2c.h>
#include <mma8x5x.h>  // Assuming mma8x5x.h defines mma8x5x_data_t
#include <mma8x5x_params.h>
#include <msg.h>

#define SLEEP_DURATION (5U * 1000000UL)

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = 0x1D,  // Assume this as the I2C address since MMA8X5X_I2C_ADDRESS caused an error
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    // Initialize the accelerometer
    int result = mma8x5x_init(&dev, &params);
    if (result != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x sensor\n");
        return -1;
    }

    while (true) {
        // Read data from the accelerometer
        mma8x5x_read(&dev, &data);
        printf("X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);

        // Sleep for 5 seconds
        xtimer_usleep(SLEEP_DURATION);
    }

    return 0;
}