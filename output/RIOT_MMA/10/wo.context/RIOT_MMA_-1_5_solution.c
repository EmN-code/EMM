#include <xtimer.h>
#include <stdio.h>
#include <periph/i2c.h>
#include <mma8x5x.h>

#define ADDR        CONFIG_MMA8X5X_I2C_ADDRESS
#define RATE        MMA8X5X_RATE_50HZ
#define RANGE       MMA8X5X_RANGE_2G
#define I2C_BUS     0
#define INTERVAL    (5U * US_PER_SEC)

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void)
{
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(I2C_BUS),
        .addr = ADDR,  // Use CONFIG_MMA8X5X_I2C_ADDRESS instead
        .rate = RATE,
        .range = RANGE
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8X5X\n");
        return 1;
    }

    while (1) {
        if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {  // Check if return is successful before continuing
            printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        } else {
            printf("Error reading data\n");
        }
        xtimer_sleep(INTERVAL);
    }
    return 0;
}