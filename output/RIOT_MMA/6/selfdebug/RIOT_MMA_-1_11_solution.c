#include <stdio.h>
#include <stdlib.h>
#include "xtimer.h"
#include "thread.h"
#include "periph/i2c.h"

#define I2C_ADDR        (0x1D)  // Default I2C address for MMA8x5x
#define INTERVAL        (5U)    // Interval in seconds

static kernel_pid_t main_thread_pid;
static char stack[THREAD_STACKSIZE_DEFAULT];

static void *mma8x5x_func(void *arg) {
    (void)arg;
    // Assume that mma8x5x_init and mma8x5x_read are pre-implemented
    mma8x5x_t dev;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = I2C_ADDR,
        .rate = 50,   // For instance, 50Hz
        .range = 2    // For instance, +/- 2g
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Initialization failed!");
        return NULL;  // Stop thread.
    }

    while (1) {
        mma8x5x_data_t data;
        if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
            printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        }
        else {
            puts("Error reading data!");
        }
        xtimer_sleep(INTERVAL);
    }
    return NULL;
}

int main(void) {
    main_thread_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                                    THREAD_CREATE_STACKTEST, mma8x5x_func, NULL, "mma8x5x thread");
    return 0;
}