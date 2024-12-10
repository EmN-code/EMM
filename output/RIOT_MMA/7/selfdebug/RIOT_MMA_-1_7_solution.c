#include "periph/i2c.h"
#include "stdio.h"
#include "xtimer.h"

// Define parameters for the MMA8x5x device
static mma8x5x_t dev;
static mma8x5x_data_t data;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = 0x1C,  // I2C address for MMA8x5x, change if necessary
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

int main(void) {
    int res;
    // Initialize the MMA8X5X sensor
    res = mma8x5x_init(&dev, &params);

    if (res != MMA8X5X_OK) {
        printf("Error initializing MMA8x5x sensor\n");
        return -1;
    }
    printf("MMA8x5x sensor initialized\n");

    while (1) {
        // Read data from the sensor
        res = mma8x5x_read(&dev, &data);
        if(res == MMA8X5X_OK) {
            printf("X: %d, Y: %d, Z: %d\n", data.x_axis, data.y_axis, data.z_axis);
        } else {
            printf("Error reading data from MMA8x5x sensor\n");
        }

        // Wait 5 seconds
        xtimer_sleep(5);
    }
    return 0;
}