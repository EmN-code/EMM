#include <thread_config.h>
#include <periph_conf.h>
#include <inttypes.h>
#include <periph/gpio.h>
#include <periph/i2c.h>
#include <timex.h>
#include <ztimer.h>

#include "mma8x5x.h"

void read_mma8x5x(mma8x5x_t *dev) {
    int16_t x, y, z;
    if (mma8x5x_read_accel(dev, &x, &y, &z) == 0) {
        printf("Accel - x: %d, y: %d, z: %d\n", x, y, z);
    } else {
        printf("Failed to read acceleration data\n");
    }
}

int main(void) {
    mma8x5x_t dev;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_ADDR,
        .drv = &mma8x5x_driver,
    };

    if (mma8x5x_init(&dev, &params) != 0) {
        printf("Initialization failed\n");
        return 1;
    }

    while (1) {
        read_mma8x5x(&dev);
        ztimer_sleep(ZTIMER_SEC, 5);
    }

    return 0;
}