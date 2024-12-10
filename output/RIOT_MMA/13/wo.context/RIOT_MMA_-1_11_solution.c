#include <xtimer.h>
#include <stdio.h>
#include <stdint.h>
#include "mma8x5x.h"
#include "periph/i2c.h"

#define I2C_ADDR       CONFIG_MMA8X5X_I2C_ADDRESS
#define I2C_DEV_NUM    (0)
#define READ_INTERVAL  (5U * US_PER_SEC) // Read every 5 seconds

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(I2C_DEV_NUM),
        .addr = I2C_ADDR,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Failed to initialize MMA8x5x");
        return 1;
    }
    puts("MMA8x5x initialized");

    while (1) {
        xtimer_sleep(READ_INTERVAL / US_PER_SEC);
        if (mma8x5x_read(&dev, &data) != MMA8X5X_OK) {
            puts("Failed to read accelerometer data");
        } else {
            printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        }
    }

    return 0;
}