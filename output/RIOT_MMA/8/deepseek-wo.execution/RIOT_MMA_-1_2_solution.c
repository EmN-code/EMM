
#include "mma8x5x_params.h"
#include "evtimer_mbox.h"
#include "mma8x5x_regs.h"
#include "ztimer.h"
#include "mma8x5x.h"
#include "time_units.h"
#include "xtimer.h"
#include "evtimer_msg.h"

#define MMA8X5X_I2C_ADDRESS CONFIG_MMA8X5X_I2C_ADDRESS
#define MMA8X5X_RATE_1_56HZ MMA8X5X_RATE_50HZ

static mma8x5x_t dev;
mma8x5x_data_t data;

static void read_accelerometer(void) {
    if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
        printf("Accelerometer data: X=%d, Y=%d, Z=%d\n", data.x, data.y, data.z);
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
