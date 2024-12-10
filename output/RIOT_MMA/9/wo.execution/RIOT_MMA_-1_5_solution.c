#include "ztimer.h"
#include "unistd.h"
#include "mma8x5x.h"
#include "xtimer.h"
#include "time_units.h"
#include "stdio.h"
#include "stdlib.h"

#define INTERVAL (5U * 1000000U) // 5 seconds in microseconds

static mma8x5x_t dev;
mma8x5x_data_t data;

int main(void) {
    // Initialize the MMA8x5x parameters
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };
    
    // Initialize the accelerometer
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize accelerometer\n");
        return 1;
    }

    printf("Accelerometer initialized successfully\n");

    while (1) {
        // Read data from the accelerometer
        mma8x5x_read(&dev, &data);

        // Print the acceleration data
        printf("Acceleration: X=%d mg, Y=%d mg, Z=%d mg\n", 
               data.x, data.y, data.z);

        // Sleep for the interval duration
        xtimer_usleep(INTERVAL);
    }

    return 0;
}