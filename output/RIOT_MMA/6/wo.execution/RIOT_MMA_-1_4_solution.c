#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdio.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"

static mma8x5x_t dev;
static mma8x5x_data_t data;

mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = CONFIG_MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void read_sensor(void) {
    int result = mma8x5x_init(&dev, &params);
    if (result != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x sensor\n");
        return;
    }

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }
}

int main(void) {
    read_sensor();
    return 0;
}