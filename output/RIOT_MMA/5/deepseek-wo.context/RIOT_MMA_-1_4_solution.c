
#include "periph/i2c.h"
#include "mma8x5x.h"
#include "evtimer_msg.h"
#include "event/thread.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "ztimer.h"
#include "evtimer_mbox.h"
#include "mbox.h"
#include "timex.h"
#include "event/periodic_callback.h"
#include "thread.h"

#define MMA8X5X_I2C_ADDRESS CONFIG_MMA8X5X_I2C_ADDRESS

static mma8x5x_t dev;
static mma8x5x_data_t data;
static msg_t msg_queue[8];

void read_accelerometer(void) {
    mma8x5x_read(&dev, &data);
    printf("Accelerometer data: x=%d, y=%d, z=%d\n", data.x, data.y, data.z);
}

int main(void) {
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

    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    evtimer_t evtimer;
    evtimer_init_msg(&evtimer);

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 5000);
        read_accelerometer();
    }

    return 0;
}
