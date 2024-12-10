
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <errno.h>

#include "evtimer_msg.h"
#include "event/thread.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "ztimer.h"
#include "mma8x5x.h"
#include "xtimer.h"
#include "evtimer_mbox.h"

#define MMA8X5X_I2C_ADDRESS CONFIG_MMA8X5X_I2C_ADDRESS
#define MMA8X5X_RATE_1_56HZ MMA8X5X_RATE_50HZ

static mma8x5x_t dev;
static mma8x5x_data_t data;

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

    int res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x accelerometer: %d\n", res);
        return 1;
    }

    evtimer_t timer;
    evtimer_init_msg(&timer);

    while (1) {
        ztimer_set(ZTIMER_MSEC, &timer, 5000);
        msg_t msg;
        msg_receive(&msg);
        read_accelerometer(NULL);
    }

    return 0;
}
