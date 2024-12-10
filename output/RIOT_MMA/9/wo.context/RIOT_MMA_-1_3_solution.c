#include <xtimer.h>
#include <stdio.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    // Initialize parameters for the MMA8x5x sensor
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS_DEFAULT,  // Use default address
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    // Initialize the sensor
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Error initializing MMA8x5x\n");
        return 1;
    }

    while (1) {
        // Read data from the sensor
        if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
            printf("Acceleration X: %d | Y: %d | Z: %d\n", data.x, data.y, data.z);
        }
        else {
            printf("Error reading data from MMA8x5x\n");
        }
        xtimer_sleep(5);  // Sleep for 5 seconds
    }
    return 0;
}