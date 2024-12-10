
#include "mma8x5x.h"
#include "unistd.h"
#include "dht_params.h"
#include "periph/rtc.h"
#include "xtimer.h"
#include "assert.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "event/periodic_callback.h"
#include "net/sock/udp.h"
#include "mma8x5x_params.h"
#include "timex.h"
#include "periph/i2c.h"
#include "mbox.h"
#include "net/sock/util.h"
#include "evtimer_msg.h"
#include "stdio.h"
#include "evtimer_mbox.h"
#include "MQTTClient.h"
#include "ztimer.h"
#include "event/thread.h"
#include "thread.h"
#include "time_units.h"
#include "mma8x5x_regs.h"
#include "kernel_defines.h"
#include "sys/socket.h"
#include "periph/gpio.h"
#include "stdbool.h"

#define INTERVAL_SEC (5U)

static mma8x5x_t dev;
static mma8x5x_data_t data;
static event_periodic_callback_t periodic_event;

static void read_accelerometer(void *arg)
{
    (void)arg;
    mma8x5x_read(&dev, &data);
    printf("Accelerometer data: x=%d, y=%d, z=%d\n", data.x, data.y, data.z);
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
        puts("Initialization of MMA8x5x failed");
        return 1;
    }

    event_periodic_callback_init(&periodic_event, ZTIMER_MSEC, &event_queue_null, read_accelerometer, NULL);
    event_periodic_callback_start(&periodic_event, INTERVAL_SEC * MS_PER_SEC);

    while (1) {
        event_queue_poll(&event_queue_null);
    }

    return 0;
}
