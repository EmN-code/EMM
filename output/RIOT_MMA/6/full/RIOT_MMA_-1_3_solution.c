#include <ztimer.h>
#include <xtimer.h>
#include <time_units.h>
#include <thread.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <mbox.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>
#include <event/thread.h>
#include <dht_params.h>

#include "periph/i2c.h"
#include <stdio.h>

#define READ_PERIOD (5LU * US_PER_SEC)  // 5 seconds

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    int rc;
    while (1) {
        rc = mma8x5x_init(&dev, &params);
        if (rc == MMA8X5X_OK) {
            printf("Accelerometer initialized successfully\n");
            break;
        } else {
            printf("Initialization failed, retrying...\n");
            ztimer_sleep(ZTIMER_SEC, 1);
        }
    }

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration data: x = %d mg, y = %d mg, z = %d mg\n",
               data.x, data.y, data.z);
        ztimer_sleep(ZTIMER_USEC, READ_PERIOD);
    }
    return 0;
}