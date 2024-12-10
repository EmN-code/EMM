#include <thread.h>
#include <stdio.h>
#include <stdint.h>
#include "mma8x5x.h"
#include "timer.h"

#define SLEEP_INTERVAL (5LU * USLEEP_CONST)

static void *mma8x5x_thread(void *arg) {
    mma8x5x_t *mma_device = (mma8x5x_t *)arg;
    while (1) {
        if (mma8x5x_is_ready(mma_device)) {
            int16_t x, y, z;
            mma8x5x_read_accel(mma_device, &x, &y, &z);
            printf("Accel: x=%d, y=%d, z=%d\n", x, y, z);
        }
        thread_sleep(SLEEP_INTERVAL);
    }
    return NULL;
}

int main(void) {
    mma8x5x_t dev;
    mma8x5x_params_t params = {
        // Initialize with your device's I2C parameters
    };

    if (mma8x5x_init(&dev, &params) != 0) {
        printf("Initialization failed\n");
        return 1;
    }

    kernel_pid_t pid = thread_create(
        malloc(THREAD_STACKSIZE_MAIN), THREAD_STACKSIZE_MAIN,
        THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST,
        mma8x5x_thread, &dev, "mma8x5x");

    if (pid == KERNEL_PID_UNDEF) {
        printf("Thread creation failed\n");
        return 1;
    }

    return 0;
}