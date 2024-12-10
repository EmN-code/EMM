#include "periph/i2c.h"
#include "kernel_defines.h"
#include "event/timeout.h"
#include <stdio.h>

static mma8x5x_t dev;
static mma8x5x_data_t data;
static ztimer_t timer;

void read_sensor(void)
{
    if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
        printf("Acceleration X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
    }
    else {
        printf("Failed to read data\n");
    }
}

int main(void)
{
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };
    
    if (mma8x5x_init(&dev, &params) == MMA8X5X_OK) {
        printf("MMA8x5x initialized\n");
    }
    else {
        printf("Failed to initialize MMA8x5x\n");
        return 1;
    }

    ztimer_set_wakeup(ZTIMER_MSEC, &timer, 5000);
    timer.callback = read_sensor;

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 5000);
        read_sensor();
    }

    return 0;
}
