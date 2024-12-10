#include "xtimer.h"
#include "stdio.h"
#include "periph/i2c.h"
#include "mma8x5x_params.h"

#define CONFIG_MMA8X5X_I2C_ADDRESS 0x1D // Default I2C address based on context suggestion

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    // Initialize the MMA8x5x sensor
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize the sensor!\n");
        return 1;
    }

    while (1) {
        // Read accelerometer data
        if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
            printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        } else {
            printf("Failed to read the sensor!\n");
        }

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}