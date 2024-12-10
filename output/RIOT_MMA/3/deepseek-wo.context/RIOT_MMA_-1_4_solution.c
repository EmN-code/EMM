
#include "dht_params.h"
#include "mma8x5x.h"
#include "evtimer_mbox.h"
#include "net/emcute.h"
#include "kernel_defines.h"
#include "assert.h"
#include "string.h"
#include "time_units.h"
#include "evtimer_msg.h"
#include "event/thread.h"
#include "mbox.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "periph/rtc.h"
#include "periph/gpio.h"
#include "timex.h"
#include "stdio.h"
#include "periph/i2c.h"
#include "stdbool.h"
#include "msg.h"
#include "ztimer.h"
#include "event/periodic_callback.h"
#include "xtimer.h"
#include "thread.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static mma8x5x_t dev;
static mma8x5x_data_t data;

static void _read_accelerometer(void)
{
    mma8x5x_read(&dev, &data);
    printf("Accelerometer data: x=%d, y=%d, z=%d\n", data.x, data.y, data.z);
}

int main(void)
{
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS,  // Corrected from MMA8X5X_I2C_ADDRESS
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Failed to initialize MMA8x5x accelerometer");
        return 1;
    }

    while (1) {
        _read_accelerometer();
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}
