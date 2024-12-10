
#include <stdio.h>  // Include the standard I/O library to resolve the implicit declaration of 'printf'
#include "periph/i2c.h"
#include "mma8x5x.h"
#include "xtimer.h"

#define MMA8X5X_I2C_ADDRESS 0x1D  // Assuming this is the default I2C address for MMA8653
#define SLEEP_SECONDS 5

int main(void)
{
    mma8x5x_t dev;
    mma8x5x_data_t data;

    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    // Initialize the MMA8x5x accelerometer
    int ret = mma8x5x_init(&dev, &params);
    if (ret != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x accelerometer: %d\n", ret);
        return 1;
    }

    while (1) {
        // Read accelerometer data
        mma8x5x_read(&dev, &data);

        // Print the accelerometer data
        printf("Accelerometer data: X=%d, Y=%d, Z=%d\n", data.x, data.y, data.z);

        // Sleep for 5 seconds
        xtimer_sleep(SLEEP_SECONDS);
    }

    return 0;
}
