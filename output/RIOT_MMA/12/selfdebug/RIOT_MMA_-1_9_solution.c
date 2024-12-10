#include <xtimer.h>
#include <stdio.h>
#include <string.h>
#include <periph/i2c.h>
#include <mma8x5x.h>

static mma8x5x_t dev;
static mma8x5x_data_t data;

// Define timer duration (5 seconds)
#define TIMER_DURATION (5U * 1000000U) // 5 seconds in microsecond

int main(void) {
    // Configuration parameters for the MMA8x5x
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    // Initialize MMA8x5x
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Error initializing MMA8x5x\n");
        return 1;
    }

    // Continuously read MMA8x5x data every 5 seconds
    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration data: x=%d, y=%d, z=%d\n", data.x_axis, data.y_axis, data.z_axis);
        xtimer_usleep(TIMER_DURATION); // Sleep for 5 seconds
    }

    return 0;
}