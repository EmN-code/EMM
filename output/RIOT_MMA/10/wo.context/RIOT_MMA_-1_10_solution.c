#include "mma8x5x/mma8x5x.h"
#include "ztimer.h"
#include "periph/i2c.h"
#include <stdio.h>

#define INTERVAL (5U * 1000U * 1000U) // 5 seconds in microseconds

static mma8x5x_t dev;
static mma8x5x_data_t data;

void read_sensor(void)
{
    if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
        printf("X: %d, Y: %d, Z: %d\n", data.x_axis, data.y_axis, data.z_axis);
    }
    else {
        puts("Error reading sensor data");
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
    
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Error initializing MMA8x5x");
        return -1;
    }

    ztimer_now_t last_wakeup = ztimer_now(ZTIMER_USEC);

    while (1) {
        read_sensor();
        ztimer_sleep_until(ZTIMER_USEC, last_wakeup += INTERVAL);
    }

    return 0;
}