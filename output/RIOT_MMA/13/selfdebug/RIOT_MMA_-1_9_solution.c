#include "periph/i2c.h"
#include <stdio.h>
#include "xtimer.h"

#define SLEEP_INTERVAL (5U * US_PER_SEC)

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

int main(void)
{
    // Initialize the accelerometer
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x\n");
        return 1;
    }
    
    while (1) {
        // Read accelerometer data
        int16_t x, y, z;
        int status = mma8x5x_read(&dev, &x, &y, &z);
        if (status == MMA8X5X_OK) {
            printf("Acceleration data: x = %d, y = %d, z = %d\n", x, y, z);
        } else {
            printf("Error reading accelerometer data\n");
        }

        // Sleep for 5 seconds
        xtimer_sleep(SLEEP_INTERVAL);
    }
    return 0;
}