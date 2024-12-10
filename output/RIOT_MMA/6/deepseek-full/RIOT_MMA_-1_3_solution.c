
#include <stdio.h>
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "ztimer.h"
#include "mma8x5x_regs.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "dht_params.h"
#include "event/thread.h"
#include "mma8x5x_params.h"
#include "evtimer_msg.h"
#include "time_units.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static mma8x5x_t dev;
static mma8x5x_data_t data;

static void *event_loop(void *arg)
{
    (void)arg;
    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration: X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);
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
        puts("Initialization of MMA8x5x failed");
        return 1;
    }

    thread_create(
        NULL,
        THREAD_STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        event_loop,
        NULL,
        "event_loop"
    );

    return 0;
}
