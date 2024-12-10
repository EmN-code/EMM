// Description:
// This code initializes an MMA8x5x accelerometer and reads its data every 5 seconds on an ESP32 using RIOT OS.
// The accelerometer is interfaced over I2C with defined parameters.

#include <stdio.h>
#include "periph/i2c.h"
#include "mma8x5x.h"
#include "thread_arch.h"

// Device configuration: I2C and MMA8X5X related parameters
#define CONFIG_MMA8X5X_I2C_ADDRESS 0x1D     // Assuming the default I2C address
#define I2C_INTERFACE I2C_DEV(0)
#define READ_INTERVAL (5U)    // 5 seconds

// Global device descriptor
static mma8x5x_t dev;

// Function to initialize MMA8x5x device
void init_mma8x5x(void) {
     mma8x5x_params_t params = {
        .i2c = I2C_INTERFACE,
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    int res = mma8x5x_init(&dev, &params);
    if (res == MMA8X5X_OK) {
        printf("MMA8x5x initialized successfully\n");
    } else {
        printf("Failed to initialize MMA8x5x: error %d\n", res);
    }
}

// Function to read data from MMA8x5x
void read_mma8x5x(void) {
    mma8x5x_data_t data;
    if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
        printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
    } else {
        printf("Failed to read data\n");
    }
}

int main(void) {
     // Initialize the MMA8x5x
    init_mma8x5x();

    // Periodically read data from the MMA8x5x every 5 seconds
    while (1) {
        read_mma8x5x();
        xtimer_sleep(READ_INTERVAL);
    }

    return 0;
}