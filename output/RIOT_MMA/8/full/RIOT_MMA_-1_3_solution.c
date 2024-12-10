#include "ztimer.h"
#include "xtimer.h"
#include "time_units.h"
#include "thread.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "event/thread.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "dht_params.h"

#define SLEEP_TIME 5

static mma8x5x_t dev;
static mma8x5x_data_t data;
mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = CONFIG_MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

int main(void)
{
    int rc;
    while (1) {
        rc = mma8x5x_init(&dev, &params);
        if (rc == MMA8X5X_OK) {
            printf("Accelerometer initialized successfully.\n");
            break;
        } else {
            printf("Accelerometer initialization failed, retrying...\n");
            ztimer_sleep(ZTIMER_MSEC, 1000);
        }
    }

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);
        ztimer_sleep(ZTIMER_SEC, SLEEP_TIME);
    }

    return 0;
}