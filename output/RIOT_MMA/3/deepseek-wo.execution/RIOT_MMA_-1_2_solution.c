
#include "mma8x5x.h"
#include "xtimer.h"
#include "ztimer.h"
#include "evtimer_mbox.h"
#include "mma8x5x_regs.h"
#include "evtimer_msg.h"
#include "mma8x5x_params.h"
#include "time_units.h"

#define MMA8X5X_I2C_ADDRESS CONFIG_MMA8X5X_I2C_ADDRESS
#define MMA8X5X_RATE_1_56HZ MMA8X5X_RATE_50HZ

static mma8x5x_t dev;
mma8x5x_data_t data;

static const mma8x5x_params_t params = {
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
        res = mma8x5x_read(&dev, &data);
        if (res == MMA8X5X_OK) {
            printf("Accelerometer data: x = %d, y = %d, z = %d\n", data.x, data.y, data.z);
        } else {
            printf("Failed to read accelerometer data\n");
        }

        xtimer_sleep(5);
    }

    return 0;
}
