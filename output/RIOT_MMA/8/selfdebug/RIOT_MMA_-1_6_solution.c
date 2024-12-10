#include <thread.h>
#include <stdio.h>
#include <periph/i2c.h>

#include "mma8x5x.h"

static mma8x5x_t dev;
static mma8x5x_data_t data;

void *read_mma8x5x(void *arg) {
    (void)arg; // Suppress unused parameter warning
    
    while (1) {
        if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
            printf("X: %d, Y: %d, Z: %d\n", data.accel[0], data.accel[1], data.accel[2]); // Correct members of mma8x5x_data_t
        } else {
            printf("Error reading data\n");
        }
        xtimer_sleep(5);  // Sleep for 5 seconds
    }
    return NULL;
}

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x\n");
        return 1;
    }

    thread_create(NULL, 0, THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, read_mma8x5x, NULL, "mma8x5x_reader");

    while (1) {
        thread_yield();
    }

    return 0;
}