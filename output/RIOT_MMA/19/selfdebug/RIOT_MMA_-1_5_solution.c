#include "periph/i2c.h"
#include "kernel_defines.h"
#include <stdio.h>

#define CONFIG_MMA8X5X_I2C_ADDRESS 0x1D // Define the correct I2C address

static mma8x5x_t dev;

int main(void) {
    printf("Starting MMA8x5x sensor reading on ESP32\n");

    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS, // Use the correct address macro
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    int init_result = mma8x5x_init(&dev, &params);
    if (init_result != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x sensor\n");
        return 1;
    }

    mma8x5x_data_t data;

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration Data X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);
        ztimer_sleep(ZTIMER_MSEC, 5000); // Sleep for 5 seconds
    }

    return 0;
}