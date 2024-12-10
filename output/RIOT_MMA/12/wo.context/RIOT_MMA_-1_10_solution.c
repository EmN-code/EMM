#include <time_units.h>
#include <periph/i2c.h>
#include <stdio.h>
#include <xtimer.h>

#define I2C_BUS 0

static mma8x5x_t dev;
static mma8x5x_data_t data;

static const mma8x5x_params_t params = {
    .i2c = I2C_DEV(I2C_BUS),
    .addr = CONFIG_MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

int main(void) {
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Initialization failed\n");
        return 1;
    }

    while (1) {
        xtimer_sleep(5);

        if (mma8x5x_read(&dev, &data) != MMA8X5X_OK) {
            printf("Failed to read data\n");
        } else {
            printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        }
    }
    return 0;
}