#include <xtimer.h>
#include <stdio.h>
#include <mma8x5x.h>

int main(void) {
    // Define and initialize accelerometer parameters
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };
    
    mma8x5x_t dev;
    mma8x5x_data_t data;

    // Initialize the accelerometer
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize the accelerometer\n");
        return 1;
    }

    printf("Accelerometer initialized\n");

    while (1) {
        // Read data from the accelerometer
        mma8x5x_read(&dev, &data);

        // Print the acceleration values
        printf("X: %d\tY: %d\tZ: %d\n", data.x, data.y, data.z);
        
        // Sleep for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}