
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

#define MMA8X5X_I2C_ADDRESS (0x1C)
#define MMA8X5X_RATE_50HZ (0x06)
#define MMA8X5X_RANGE_2G (0x00)

static mma8x5x_t dev;
static mma8x5x_data_t data;

static void read_accelerometer(void) {
    if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
        printf("Accelerometer data: x=%d, y=%d, z=%d\n", data.x, data.y, data.z);
    } else {
        printf("Failed to read accelerometer data\n");
    }
}

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x accelerometer\n");
        return 1;
    }

    while (1) {
        read_accelerometer();
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}
