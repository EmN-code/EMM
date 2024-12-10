#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "event/thread.h"

#include <stdio.h>
#include "xtimer.h"
#include "periph/i2c.h"

#define ADDRESS 0x1C  // You should define the accelerometer address if not defined

static mma8x5x_t dev;
static mma8x5x_data_t data;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

int main(void)
{
    int rc;

    printf("Starting MMA8x5x accelerometer application\n");

    // Initialize the accelerometer
    while (1) {
        rc = mma8x5x_init(&dev, &params);
        if (rc == MMA8X5X_OK) {
            printf("Accelerometer initialized successfully\n");
            break;
        } else if (rc == MMA8X5X_NOI2C) {
            printf("Error: I2C bus initialization failed\n");
            xtimer_sleep(1);
            continue;
        } else if (rc == MMA8X5X_NODEV) {
            printf("Error: Accelerometer device test failed\n");
            xtimer_sleep(1);
            continue;
        } else {
            printf("Unknown error occurred\n");
            xtimer_sleep(1);
            continue;
        }
    }

    while (1) {
        // Read accelerometer data
        if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
            printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        } else {
            printf("Failed to read data\n");
        }

        xtimer_sleep(5);
    }

    return 0;
}