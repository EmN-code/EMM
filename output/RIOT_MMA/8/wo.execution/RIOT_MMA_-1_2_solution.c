#include "thread.h"
#include "periph_conf.h"
#include "periph/rtc.h"
#include "periph/i2c.h"
#include "mma8x5x_regs.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"
#include <stdio.h>

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_8G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x\n");
        return -1;
    }

    while (1) {
        if (mma8x5x_is_ready(&dev)) {
            mma8x5x_read(&dev, &data);
            
            int x = (data.x * 1000) / 256;
            int y = (data.y * 1000) / 256;
            int z = (data.z * 1000) / 256;

            printf("Acceleration: x=%d mg, y=%d mg, z=%d mg\n", x, y, z);
        }
        xtimer_sleep(5);
    }
    return 0;
}