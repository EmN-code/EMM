// Required header files
#include "ztimer.h"
#include "mma8x5x.h"
#include "kernel.h"
#include <stdio.h>

// Define device and parameters
static mma8x5x_t dev;
static mma8x5x_data_t data;
mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

// Timer callback function
void read_sensor(void *arg) {
    if (mma8x5x_init(&dev, &params) == MMA8X5X_OK) {
        mma8x5x_read(&dev, &data);
        printf("X=%d, Y=%d, Z=%d\n", data.x, data.y, data.z);
    }
    else {
        printf("Failed to initialize MMA8x5x sensor\n");
    }
}

// Main function
int main(void) {
    // Initialize the sensor
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x sensor\n");
        return 1;
    }

    // Set a periodic timer to read the sensor every 5 seconds
    ztimer_periodic(&ZTIMER_MSEC, read_sensor, NULL, 5000);

    // Keep the program running
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}