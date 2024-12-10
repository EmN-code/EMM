
#include <stdio.h>
#include <xtimer.h>
#include <mma8x5x.h>

#define SLEEP_INTERVAL_MS (5 * 1000U)  // 5 seconds

static mma8x5x_t dev;
static const mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .mode = MMA8X5X_MODE_NORMAL,
    .rate = MMA8X5X_RATE_1_56HZ,
    .range = MMA8X5X_RANGE_2G
};

int main(void)
{
    // Initialize the MMA8x5x sensor
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Failed to initialize MMA8x5x sensor");
        return 1;
    }

    while (1) {
        // Read the accelerometer data
        mma8x5x_data_t data;
        if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
            printf("Accelerometer data: X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        } else {
            puts("Failed to read accelerometer data");
        }

        // Sleep for 5 seconds
        xtimer_sleep(SLEEP_INTERVAL_MS / 1000);
    }

    return 0;
}
