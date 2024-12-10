
#include <stdio.h>
#include <assert.h>
#include "thread.h"
#include "xtimer.h"
#include "event/periodic_callback.h"
#include "dht_params.h"
#include "ztimer.h"
#include "mbox.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "mma8x5x_regs.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static mma8x5x_t dev;
static mma8x5x_data_t data;

static void *sensor_thread(void *arg)
{
    (void)arg;

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration: X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void)
{
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    int res = mma8x5x_init(&dev, &params);
    assert(res == MMA8X5X_OK);

    kernel_pid_t pid = thread_create(
        thread_stack, sizeof(thread_stack),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        sensor_thread, NULL, "sensor_thread");

    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        // Handle messages if needed
    }

    return 0;
}
