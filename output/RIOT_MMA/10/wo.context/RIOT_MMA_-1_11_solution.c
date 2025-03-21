#include "periph/i2c.h"
#include "mma8x5x/mma8x5x.h"
#include <xtimer.h>

#define INTERVAL (5U * 1000000U) // 5 seconds in microseconds

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    // Initialize MMA8x5x parameters
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    // Initialize the sensor
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x sensor!\n");
        return -1;
    }

    while (1) {
        // Read sensor data
        mma8x5x_read(&dev, &data);
        printf("X: %d mg Y: %d mg Z: %d mg\n", data.x, data.y, data.z);
        
        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}