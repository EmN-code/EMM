
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <periph/i2c.h>
#include <periph/gpio.h>
#include <xtimer.h>
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"

#define MMA8X5X_I2C_ADDRESS CONFIG_MMA8X5X_I2C_ADDRESS
#define SLEEP_INTERVAL (5 * 1000 * 1000U) // 5 seconds in microseconds

static mma8x5x_t dev;
static mma8x5x_data_t data;

void read_sensor(void) {
    int ret = mma8x5x_read(&dev, &data);
    if (ret == MMA8X5X_OK) {
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

    int ret = mma8x5x_init(&dev, &params);
    if (ret != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x accelerometer\n");
        return 1;
    }

    while (1) {
        read_sensor();
        xtimer_usleep(SLEEP_INTERVAL);
    }

    return 0;
}
