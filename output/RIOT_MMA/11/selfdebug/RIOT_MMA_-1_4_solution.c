#include "ztimer.h"
#include "periph/i2c.h"
#include "mma8x5x.h"
#include <stdio.h>

#define I2C_DEV (I2C_DEV(0))
#define ADDR (0x1D)  // Default I2C address for MMA8653
#define RATE (MMA8X5X_RATE_50HZ)
#define RANGE (MMA8X5X_RANGE_2G)

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV,
    .addr = ADDR,
    .rate = RATE,
    .range = RANGE
};

static void read_sensor(void)
{
    mma8x5x_data_t data;
    if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
        printf("Acceleration: X=%d, Y=%d, Z=%d\n", data.x, data.y, data.z);
    }
    else {
        printf("Failed to read data from sensor\n");
    }
}

int main(void)
{
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize sensor\n");
        return 1;
    }
    
    ztimer_t timer;
    ztimer_set(&timer, ZTIMER_MSEC, 5000, read_sensor, NULL);

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
    return 0;
}