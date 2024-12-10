#include "periph_conf.h"
#include "periph/i2c.h"
#include "kernel_defines.h"
#include "mma8x5x.h"

#define MMA8X5X_ADDR   (0x1D)
#define DELAY_INTERVAL (5U)

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_ADDR,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void read_accel(void)
{
    int16_t acc_data[3];
    if (mma8x5x_read(&dev, acc_data) == MMA8X5X_OK) {
        printf("Accel Data: X=%d, Y=%d, Z=%d\n", acc_data[0], acc_data[1], acc_data[2]);
    }
    else {
        printf("Error reading accel data\n");
    }
}

int main(void)
{
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Initialization failed\n");
        return 1;
    }

    while (1) {
        read_accel();
        xtimer_sleep(DELAY_INTERVAL);
    }

    return 0;
}