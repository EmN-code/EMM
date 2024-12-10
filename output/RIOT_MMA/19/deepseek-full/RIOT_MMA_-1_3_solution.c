
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#include "evtimer_mbox.h"
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
    msg_t msg;
    msg_t msg_queue[MAIN_QUEUE_SIZE];
    msg_init_queue(msg_queue, MAIN_QUEUE_SIZE);

    while (1) {
        msg_receive(&msg);
        mma8x5x_read(&dev, &data);
        printf("Acceleration [mg]: X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
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

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Initialization failed");
        return 1;
    }

    kernel_pid_t pid = thread_create(
        _main_msg_queue, sizeof(_main_msg_queue), THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST, event_loop, NULL, "event_loop");

    evtimer_mbox_t et;
    evtimer_mbox_init(&et, _main_msg_queue);

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 5000);
        msg_t msg;
        msg.content.ptr = NULL;
        evtimer_mbox_add_msg(&et, &msg, pid);
    }

    return 0;
}
