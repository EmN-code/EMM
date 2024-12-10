#include <ztimer.h>  // Provides timer functionalities which are essential for scheduling tasks in RTOS
#include <xtimer.h>  // For precise timing requirements
#include <timex.h>  // Offers data structures and functions for time handling
#include <sys/types.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>
#include "mma8x5x.h" // Interface for the MMA8x5x accelerometer
#include "mma8x5x_params.h" 

// Definitions for GPIO, I2C or other hardware specific settings
#define CONFIG_MMA8X5X_I2C_ADDRESS 0x1D // Default I2C address for MMA8x5x
#define I2C_DEV(0) 0  // Assuming that the I2C device index is 0

typedef struct {
    int x, y, z;  // For storing accelerometer readings
} mma8x5x_data_t;

static mma8x5x_t dev;

mma8x5x_data_t data;
mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = CONFIG_MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void read_accelerometer(void)
{
    // Check if the accelerometer is ready before reading
    if (mma8x5x_is_ready(&dev)) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration data: X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);
    }
    else {
        printf("Accelerometer not ready!\n");
    }
}

int main(void)
{
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize accelerometer\n");
        return 1;
    }

    while (1) {
        read_accelerometer();
        ztimer_sleep(ZTIMER_USEC, 5000000);  // Sleep for 5 seconds
    }

    return 0;
}