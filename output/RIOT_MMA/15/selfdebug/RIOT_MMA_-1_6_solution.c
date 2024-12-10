#include "xtimer.h"
#include "mma8x5x.h"
#include "periph/i2c.h"
#include <stdio.h>

#define INTERVAL (5U * US_PER_SEC) // 5 seconds interval

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    printf("Starting MMA8x5x accelerometer read example\n");

    // Define parameters for the accelerometer
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    // Initialize the accelerometer
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Error: Unable to initialize accelerometer\n");
        return 1;
    }

    while (1) {
        if (mma8x5x_read(&dev, &data) != MMA8X5X_OK) {
            printf("Error reading accelerometer data\n");
        }
        else {
            printf("Acceleration [X/Y/Z]: %d/%d/%d\n", data.raw[0], data.raw[1], data.raw[2]);
        }

        xtimer_sleep(INTERVAL);
    }

    return 0;
}