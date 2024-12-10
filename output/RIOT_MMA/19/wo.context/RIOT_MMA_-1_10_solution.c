#include "ztimer.h"
#include "periph/i2c.h"
#include "xtimer.h"
#include <stdio.h>

#include "mma8x5x.h" // Include the MMA8x5x sensor driver

#define I2C_DEV        0  // Define the I2C bus for ESP32, assuming bus 0 is used
#define US_PER_SEC     1000000 // Define microseconds per second if not defined

static mma8x5x_t dev; // Sensor device descriptor

int main(void) {
    i2c_t i2c = I2C_DEV;
    mma8x5x_params_t params = {
        .i2c = i2c,
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Initialization failed
");
        return 1;
    }
    puts("MMA8x5x sensor initialized
");

    mma8x5x_data_t data;

    while (1) {
        if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
            printf("X: %d, Y: %d, Z: %d
", data.x_axis, data.y_axis, data.z_axis);
        } else {
            puts("Error reading data
");
        }
        xtimer_usleep(5 * US_PER_SEC); // Sleep for 5 seconds
    }

    return 0;
}