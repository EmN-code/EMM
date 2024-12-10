
#include "ztimer.h"
#include "stddef.h"
#include "stdbool.h"
#include "msg.h"
#include "dht.h"
#include "evtimer_mbox.h"
#include "timex.h"
#include "kernel_defines.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "stdlib.h"
#include "mbox.h"
#include "stdio.h"
#include "xtimer.h"
#include "mma8x5x_regs.h"
#include "unistd.h"
#include "stdatomic.h"
#include "mma8x5x.h"
#include "dht_params.h"
#include "mma8x5x_params.h"

#define SENSOR_READ_INTERVAL (5 * US_PER_SEC)  // 5 seconds

static mbox_t mbox;
static msg_t msg_queue[8];
static evtimer_mbox_event_t event;
static evtimer_t evtimer;

void sensor_read_task(void)
{
    mma8x5x_data_t data;
    int res = mma8x5x_read(&dev, &data);
    if (res == MMA8X5X_OK) {
        printf("Accelerometer data: x=%d, y=%d, z=%d\n", data.x, data.y, data.z);
    } else {
        printf("Failed to read accelerometer data\n");
    }
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
    if (res != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x accelerometer\n");
        return 1;
    }

    mbox_init(&mbox, msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));
    evtimer_init(&evtimer);

    event.mbox = &mbox;
    event.event.offset = 0;
    event.event.interval = SENSOR_READ_INTERVAL;
    event.event.callback = (evtimer_callback_t)sensor_read_task;

    evtimer_add_mbox(&evtimer, &event, &mbox);

    while (1) {
        msg_t msg;
        mbox_get(&mbox, &msg);
        sensor_read_task();
    }

    return 0;
}
