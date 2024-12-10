#include "xtimer.h"
#include "stdlib.h"
#include "stdio.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"

// Define a stack size for the thread
#define THREAD_STACKSIZE_MAIN    (4096)

static kernel_pid_t main_thread_pid;

void *mma8x5x_reader(void *arg) {
    mma8x5x_t dev;
    mma8x5x_data_t data;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    int res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("Initialization failed\n");
        return NULL;
    }

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration [mg]: X=%d Y=%d Z=%d\n", data.x, data.y, data.z);
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    printf("Starting MMA8x5x reader thread\n");
    main_thread_pid = thread_create(NULL, THREAD_STACKSIZE_MAIN, THREAD_PRIORITY_MAIN - 1,
                                     0, mma8x5x_reader, NULL, "mma8x5x_reader");

    return 0;
}