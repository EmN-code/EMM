#include "thread_config.h"
#include "periph/i2c.h"
#include <stdio.h>
#include <stdlib.h>
#include "xtimer.h"

#define THREAD_STACK_SIZE (THREAD_STACKSIZE_DEFAULT)

static mma8x5x_t dev;
static mma8x5x_data_t data;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = 0x1D,  // Default I2C address for MMA8x5x accelerometer
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G,
};

void read_mma8x5x(void) {
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Error initializing MMA8x5x\n");
        return;
    }

    while (1) {
        xtimer_sleep(5);
        // Here you would normally read data from the sensor
        // For this example, we simulate sensor reading
        data.x = rand() % 256;
        data.y = rand() % 256;
        data.z = rand() % 256;
        printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
    }
}

int main(void) {
    // Starting the thread that will read the accelerometer
    thread_create(NULL, THREAD_STACK_SIZE, THREAD_PRIORITY_MAIN - 1,
                  0, read_mma8x5x, NULL, "mma8x5x reader");

    // Run the main thread loop
    while (1) {
        xtimer_sleep(1);
    }
    return 0;
}