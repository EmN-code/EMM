#include <stdio.h>
#include <stdint.h>
#include <periph/gpio.h>
#include <kernel_types.h>
#include <thread.h>
#include "xtimer.h"

#define THREAD_STACKSIZE_MAIN  THREAD_STACKSIZE_DEFAULT
#define THREAD_PRIORITY_MAIN   (THREAD_PRIORITY_DEFAULT - 1)
#define THREAD_CREATE_STACKTEST (8)

static char stack[THREAD_STACKSIZE_MAIN];
static kernel_pid_t thread_pid;

void *mma8x5x_thread(void *arg) {
    (void)arg;
    mma8x5x_t dev;
    mma8x5x_data_t data;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };
    
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Initialization failed!\n");
        return NULL;
    }

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration X: %d mg, Y: %d mg, Z: %d mg\n", data.x_axis, data.y_axis, data.z_axis);
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    puts("Starting MMA8x5x accelerometer example...");

    thread_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST,
                               mma8x5x_thread, NULL, "mma8x5x_thread");
    
    if (thread_pid < 0) {
        printf("Thread creation failed!\n");
        return 1;
    }

    return 0;
}
