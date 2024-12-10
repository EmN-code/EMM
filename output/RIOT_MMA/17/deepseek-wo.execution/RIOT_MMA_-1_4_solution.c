
#include <stdlib.h>
#include <mma8x5x_regs.h>
#include <msg.h>
#include <mma8x5x.h>
#include <time_units.h>
#include <xtimer.h>
#include <thread.h>
#include <mma8x5x_params.h>
#include <dht_params.h>
#include <stdio.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <dht.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <evtimer_mbox.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <unistd.h>
#include <stddef.h>
#include <mbox.h>
#include <event/thread.h>

#define MMA8X5X_I2C_ADDRESS 0x1D
#define MMA8X5X_RATE_50HZ 0x06
#define MMA8X5X_RANGE_2G 0x00

static mma8x5x_t dev;
mma8x5x_data_t data;

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    int result = mma8x5x_init(&dev, &params);
    if (result != MMA8X5X_OK) {
        printf("MMA8x5x initialization failed with error code %d\n", result);
        return 1;
    }

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Accelerometer data: x = %d, y = %d, z = %d\n", data.x, data.y, data.z);
        xtimer_sleep(5);
    }

    return 0;
}
