#include "thread_arch.h"
#include "thread.h"
#include "periph_conf.h"
#include "mma8x5x.h"

int main(void) {
    mma8x5x_t dev;
    mma8x5x_data_t data;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Error: could not initialize MMA8x5x\n");
        return 1;
    }

    while (1) {
        mma8x5x_read(&dev, &data);

        printf("X: %d\n", data.x);
        printf("Y: %d\n", data.y);
        printf("Z: %d\n", data.z);

        xtimer_sleep(5);
    }

    return 0;
}