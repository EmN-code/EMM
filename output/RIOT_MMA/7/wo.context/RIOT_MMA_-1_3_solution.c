#include <stdio.h>
#include "periph/i2c.h"
#include "xtimer.h"
#include "mma8x5x.h"

#define I2C_DEV         I2C_DEV(0)
#define MMA8X5X_ADDR    CONFIG_MMA8X5X_I2C_ADDRESS
#define READ_INTERVAL   (5U * US_PER_SEC)

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void)
{
    mma8x5x_params_t params;
    params.i2c = I2C_DEV;
    params.addr = MMA8X5X_ADDR;
    params.rate = MMA8X5X_RATE_50HZ;
    params.range = MMA8X5X_RANGE_2G;

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x\n");
        return 1;
    }
    printf("MMA8x5x initialized\n");

    while (1) {
        if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
            printf("Acceleration [mg]: X=%.2f Y=%.2f Z=%.2f\n",
                   data.x_axis, data.y_axis, data.z_axis);
        }
        else {
            printf("Failed to read from MMA8x5x\n");
        }
        xtimer_sleep(5);
    }
    return 0;
}