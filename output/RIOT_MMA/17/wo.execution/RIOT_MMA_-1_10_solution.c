#include <xtimer.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "mma8x5x.h"
#include "arpa/inet.h"
#include "kernel_defines.h"
#include "mma8x5x_params.h"
#include "msg.h"
#include "thread.h"
#include "thread_arch.h"
#include "timex.h"
#include "unistd.h"
#include "ztimer.h"

// MMA8X5X_I2C_ADDRESS is typically defined in the driver settings or a configuration file
#define MMA8X5X_I2C_ADDRESS 0x1D

static mma8x5x_t dev;
static mma8x5x_data_t data;

static void *read_sensor(void *arg) {
    (void)arg; // Avoid unused parameter warning
    
    while (1) {
        if (mma8x5x_is_ready(&dev)) {
            mma8x5x_read(&dev, &data);
            printf("X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);
        }
        else {
            printf("Sensor not ready\n");
        }
        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    // Set the parameters for the accelerometer
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };
    
    // Initialize the accelerometer
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize accelerometer\n");
        return 1;
    }

    printf("Accelerometer initialized successfully\n");

    // Create a thread to read the sensor
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, read_sensor, NULL, "read_sensor");

    // The main thread can perform other tasks or enter an idle state
    while (1) {
        xtimer_sleep(60); // Just to keep the main thread alive
    }

    return 0;
}