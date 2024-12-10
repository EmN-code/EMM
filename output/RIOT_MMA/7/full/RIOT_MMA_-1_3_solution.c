#include "ztimer.h"
#include "mma8x5x_params.h"
#include "dht_params.h"
#include "timex.h"
#include "thread.h"
#include "mma8x5x.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "mma8x5x_regs.h"
#include "event/thread.h"
#include "xtimer.h"
#include "time_units.h"

static mma8x5x_t dev;
static mma8x5x_data_t data;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = CONFIG_MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void read_accelerometer(void) {
    while (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Failed to initialize accelerometer. Retrying...");
        ztimer_sleep(ZTIMER_SEC, 1);
    }

    while (1) {
        if (mma8x5x_is_ready(&dev)) {
            mma8x5x_read(&dev, &data);
            printf("Accel(x) = %d, Accel(y) = %d, Accel(z) = %d\n",
                   data.x, data.y, data.z);
        } else {
            puts("Accelerometer not ready");
        }
        ztimer_sleep(ZTIMER_SEC, 5);
    }
}

int main(void) {
    read_accelerometer();
    return 0;
}