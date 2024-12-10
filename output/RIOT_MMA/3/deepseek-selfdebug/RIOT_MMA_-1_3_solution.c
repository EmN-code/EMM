
#include "periph/i2c.h"
#include "mma8x5x.h"
#include "xtimer.h"
#include <stdio.h>

#define MMA8X5X_I2C_ADDRESS (0x1D)  // Assuming this is the default I2C address for MMA8653
#define SLEEP_INTERVAL (5 * 1000 * 1000U)  // 5 seconds in microseconds

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

int main(void)
{
    int res;
    mma8x5x_data_t data;

    // Initialize the MMA8x5x accelerometer
    res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x accelerometer: %d\n", res);
        return 1;
    }

    while (1) {
        // Read accelerometer data
        res = mma8x5x_read(&dev, &data);
        if (res == MMA8X5X_OK) {
            printf("Accelerometer data: x = %d, y = %d, z = %d\n", data.x, data.y, data.z);
        } else {
            printf("Failed to read accelerometer data: %d\n", res);
        }

        // Sleep for 5 seconds
        xtimer_usleep(SLEEP_INTERVAL);
    }

    return 0;
}
