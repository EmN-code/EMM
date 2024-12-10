#include <thread.h>
#include <periph_conf.h>
#include <stdio.h>
#include <xtimer.h>
#include "mma8x5x.h"

// MMA8x5x device parameters (adjustable based on real use-case)
static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c_dev = I2C_DEV(0), // Assuming I2C 0 is used
    .i2c_addr = MMA8X5X_ADDR(0), // Default address or adjusted if needed
    .scale = MMA8X5X_SCALE_2G, // Scale set as per requirement
};

int main(void) {
    puts("Initializing MMA8x5x sensor...");

    // Initialize the sensor
    if (mma8x5x_init(&dev, &params) != 0) {
        puts("Sensor initialization failed!");
        return 1;
    }

    puts("Sensor initialized successfully.");

    // Set up the timer for periodic readings
    while (1) {
        xtimer_sleep(5); // Sleep for 5 seconds

        // Read data from the sensor
        if (mma8x5x_read(&dev) != 0) {
            puts("Failed to read data from the sensor.");
            continue;
        }

        // Retrieve and print the sensor data
        int16_t x, y, z;
        x = mma8x5x_get_x(&dev);
        y = mma8x5x_get_y(&dev);
        z = mma8x5x_get_z(&dev);

        printf("Acceleration (x, y, z): (%d, %d, %d)\n", x, y, z);
    }

    return 0;
}