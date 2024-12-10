#include <xtimer.h>
#include <stdio.h>
#include <i2c.h>
#include <mma8x5x.h>

#define SLEEP_INTERVAL (5 * 1000000U) // 5 seconds

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    printf("Initializing...");

    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Error: Failed to initialize MMA8x5x\n");
        return 1;
    }
    printf("Success: MMA8x5x initialized\n");

    while (1) {
        if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
            printf("Accelerometer data: X = %d, Y = %d, Z = %d\n", data.x, data.y, data.z);
        } else {
            printf("Error: Failed to read MMA8x5x\n");
        }
        xtimer_usleep(SLEEP_INTERVAL);
    }

    return 0;
}