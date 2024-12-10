#include <xtimer.h>
#include <stdint.h>
#include <stdio.h>
#include "periph/i2c.h"
#include "mma8x5x.h"

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    printf("Starting MMA8x5x accelerometer sensor reading on ESP32\n");

    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS, // Use config macro instead
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    int res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("Failed to initialize the MMA8x5x sensor\n");
        return 1;
    }

    while (1) {
        xtimer_sleep(5);
        res = mma8x5x_read(&dev, &data);
        if (res == MMA8X5X_OK) {
            printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        } else {
            printf("Error reading data\n");
        }
    }
    return 0;
}