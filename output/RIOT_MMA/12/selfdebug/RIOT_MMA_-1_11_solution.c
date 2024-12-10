#include <ztimer.h>
#include <thread.h>
#include <stdio.h>
#include <periph/i2c.h>
#include "mma8x5x.h"

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    printf("Initializing MMA8x5x sensor\n");
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x sensor\n");
        return 1;
    }

    while (1) {
        ztimer_periodic_wakeup(ZTIMER_MSEC, 5000);

        if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
            printf("Acceleration data: X= %d, Y= %d, Z= %d\n", data.x, data.y, data.z);
        } else {
            printf("Failed to read data from sensor\n");
        }
    }

    return 0;
}