#include <thread_config.h>
#include <periph/i2c.h>
#include <periph/gpio.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "mma8x5x.h"

#define I2C_INTERFACE I2C_DEV(0)
#define ADDRESS MMA8X5X_I2C_ADDRESS
#define RATE MMA8X5X_RATE_50HZ
#define RANGE MMA8X5X_RANGE_2G

static mma8x5x_t dev;
static mma8x5x_data_t data;

typedef struct {
    i2c_t i2c;
    uint8_t addr;
    uint8_t rate;
    uint8_t range;
    uint8_t offset[3];
} mma8x5x_params_t;

int main(void) {
    puts("Initializing MMA8x5x sensor...");

    mma8x5x_params_t params = {
        .i2c = I2C_INTERFACE,
        .addr = ADDRESS,
        .rate = RATE,
        .range = RANGE
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Failed to initialize MMA8x5x!");
        return -1;
    }

    while (1) {
        if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
            printf("X: %d, Y: %d, Z: %d\n", data.x_axis, data.y_axis, data.z_axis);
        } else {
            printf("Error reading data\n");
        }

        xtimer_sleep(5);
    }

    return 0;
}