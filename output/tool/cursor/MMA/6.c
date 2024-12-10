#include "mma8x5x.h"
#include "xtimer.h"
#include "periph/i2c.h"
#include <stdio.h>

#define I2C_DEV         I2C_DEV(0)  // Adjust based on your setup
#define MMA8X5X_ADDR    0x1D        // Default I2C address for MMA8x5x
#define INTERVAL        (5U * US_PER_SEC) // 5 seconds

static mma8x5x_t dev;
static mma8x5x_data_t data;

void read_sensor(void) {
    if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
        printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
    } else {
        printf("Failed to read data from MMA8x5x\n");
    }
}

int main(void) {
    // Initialize I2C
    i2c_init(I2C_DEV);

    // Set up MMA8x5x parameters
    mma8x5x_params_t params = {
        .i2c = I2C_DEV,
        .addr = MMA8X5X_ADDR,
        .rate = 0,  // Set appropriate rate
        .range = 0  // Set appropriate range
    };

    // Initialize the MMA8x5x device
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x\n");
        return 1;
    }

    // Main loop
    while (1) {
        read_sensor();
        xtimer_sleep(INTERVAL);
    }

    return 0;
}