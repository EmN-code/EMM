#include "xtimer.h"
#include "thread.h"
#include "periph_conf.h"
#include "periph/i2c.h"
#include "mma8x5x.h"

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Failed to initialize MMA8x5x");
        return 1;
    }

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration x: %d mg, y: %d mg, z: %d mg\n", data.x, data.y, data.z);
        xtimer_sleep(5);
    }

    return 0;
}
