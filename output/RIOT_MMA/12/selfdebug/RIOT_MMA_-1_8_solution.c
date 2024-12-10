#include "xtimer.h"
#include "periph/i2c.h"
#include "mma8x5x.h"
#include <stdio.h>

#define I2C_DEV      I2C_DEV(0)
#define ADDR         MMA8X5X_I2C_ADDRESS
#define RATE         MMA8X5X_RATE_50HZ
#define RANGE        MMA8X5X_RANGE_2G
#define SLEEP_TIME   (5U * US_PER_SEC)

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV,
        .addr = ADDR,
        .rate = RATE,
        .range = RANGE
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Initialization failed!");
        return 1;
    }

    printf("Device initialized successfully\n");

    while (1) {
        xtimer_sleep(5);

        mma8x5x_read(&dev, &data);
        printf("Acceleration data: X: %d, Y: %d, Z: %d\n",
               data.x, data.y, data.z);
    }

    return 0;
}