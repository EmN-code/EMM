#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <xtimer.h>
#include <i2c.h>
#include <mma8x5x.h>

#define I2C_DEV0 0
#define MMA8X5X_I2C_ADDRESS CONFIG_MMA8X5X_I2C_ADDRESS
#define DELAY 5U * US_PER_SEC

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(I2C_DEV0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };
    
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Error: Could not initialize MMA8x5x\n");
        return 1;
    }
    
    while (1) {
        if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
            printf("X: %d, Y: %d, Z: %d\n", data.x_axis, data.y_axis, data.z_axis);
        } else {
            printf("Error: Could not read data\n");
        }
        xtimer_sleep(DELAY);
    }
    return 0;
}