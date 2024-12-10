
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <time.h>

#include "thread.h"
#include "event/thread.h"
#include "event/periodic_callback.h"
#include "ztimer.h"
#include "mbox.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "timex.h"
#include "mma8x5x.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static mma8x5x_t dev;
static mma8x5x_data_t data;

static void *accelerometer_thread(void *arg)
{
    (void)arg;

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Accelerometer data: x: %d, y: %d, z: %d\n", data.x, data.y, data.z);
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return NULL;
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Initialization of MMA8x5x accelerometer failed");
        return 1;
    }

    puts("MMA8x5x accelerometer initialized successfully");

    thread_create(
        NULL,
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        accelerometer_thread,
        NULL,
        "accelerometer_thread"
    );

    return 0;
}
