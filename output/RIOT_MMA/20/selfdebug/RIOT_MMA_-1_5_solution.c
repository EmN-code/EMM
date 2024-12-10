#include <timex.h>
#include <thread_config.h>
#include <thread_arch.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <event.h>
#include <cpu_conf.h>
#include <periph_conf.h>
#include <periph_cpu.h>
#include "mma8x5x.h"

// Define the stack size for the thread
#define THREAD_STACKSIZE_MAIN    (1024 * 4)

// Define the priority for the thread
#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_MAIN - 1)

// Device descriptor for the accelerometer
static mma8x5x_t dev;

// Thread function to read from the accelerometer periodically
void *read_accelerometer(void *arg) {
    (void)arg; // Avoid unused parameter warning

    while (1) {
        int16_t x, y, z;
        mma8x5x_read(&dev, &x, &y, &z);
        printf("Acceleration - X: %d, Y: %d, Z: %d\n", x, y, z);
        xtimer_sleep(5); // Sleep for 5 seconds
    }

    return NULL;
}

int main(void) {
    // Define and initialize parameters for the accelerometer
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = 0x1D, // I2C address for the MMA8x5x
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    // Initialize the accelerometer
    if (mma8x5x_init(&dev, &params) != 0) {
        printf("Initialization failed\n");
        return 1;
    }

    printf("MMA8x5x initialized\n");

    // Create a thread to read accelerometer data
    thread_create_alloca(THREAD_STACKSIZE_MAIN, THREAD_PRIORITY_MAIN, 0, read_accelerometer, NULL, "accel_thread");

    // Keep the main thread alive indefinitely
    while (1) {
        thread_yield();
    }

    return 0;
}
