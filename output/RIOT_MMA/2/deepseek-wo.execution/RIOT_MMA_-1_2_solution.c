
#include "mma8x5x.h"
#include "xtimer.h"
#include "ztimer.h"
#include "evtimer_mbox.h"
#include "mma8x5x_regs.h"
#include "evtimer_msg.h"
#include "mma8x5x_params.h"
#include "event/thread.h"

#define MMA8X5X_I2C_ADDRESS CONFIG_MMA8X5X_I2C_ADDRESS
#define MMA8X5X_RATE_1_56HZ MMA8X5X_RATE_50HZ

static mma8x5x_t dev;
mma8x5x_data_t data;

mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

int main(void) {
    int res;

    res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("Initialization failed\n");
        return 1;
    }

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration: X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);
        xtimer_sleep(5);
    }

    return 0;
}
