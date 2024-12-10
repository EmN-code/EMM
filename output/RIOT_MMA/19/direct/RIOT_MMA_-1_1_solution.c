#include <xtimer.h>
#include <stdio.h>
#include "mma8x5x.h"
#include "periph/i2c.h"

#define I2C_DEV      (I2C_DEV(0))
#define I2C_ADDR     (0x1D)      // Default I2C address for the MMA8x5x

static mma8x5x_t dev;

int main(void) {
    puts("MMA8x5x Accelerometer Test");

    // Initialize I2C
    if (i2c_init(I2C_DEV) != 0) {
        puts("Error: Unable to initialize I2C");
        return 1;
    }

    // Initialize the MMA8x5x sensor
    if (mma8x5x_init(&dev, I2C_DEV, I2C_ADDR)) {
        puts("Error: Unable to initialize MMA8x5x");
        return 1;
    }

    while (1) {
        int16_t x, y, z;

        // Read accelerometer data
        if (mma8x5x_read(&dev, &x, &y, &z) == 0) {
            printf("Acceleration [X: %d Y: %d Z: %d]\n", x, y, z);
        } else {
            puts("Error: Unable to read data from MMA8x5x");
        }

        // Sleep for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}