#include "xtimer.h"
#include "ztimer.h"
#include "mma8x5x.h"
#include "msg.h"
#include "sched.h"
#include "thread.h"
#include <stdio.h>
#include <stdlib.h>

#define STACKSIZE (THREAD_STACKSIZE_MAIN)

static char stack[STACKSIZE];
static kernel_pid_t thread_pid;

static void *mma8x5x_thread(void *arg) {
    mma8x5x_t dev;
    mma8x5x_data_t data;
    const mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Initialization failed!");
        return NULL;
    }

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("x: %d mg, y: %d mg, z: %d mg\n", data.x, data.y, data.z);
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return NULL;
}

int main(void) {
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    thread_pid = thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                               THREAD_CREATE_STACKTEST, mma8x5x_thread,
                               NULL, "mma8x5x_thread");

    return 0;
}