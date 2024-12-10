#include "thread_arch.h"
#include "thread_config.h"
#include "thread_flags.h"
#include "thread.h"
#include "periph_conf.h"
#include "net/protnum.h"
#include "ble_hs_adv.h"
#include "ble_ibeacon.h"

#include <stdio.h>
#include <xtimer.h>  // For the delay function
#include <mma8x5x.h> // Include the mma8x5x sensor library

#define SLEEP_INTERVAL (5U * US_PER_SEC) // 5 seconds sleep interval

int main(void) {
    mma8x5x_t dev;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_ADDR(0),
    };

    if (mma8x5x_init(&dev, &params) != 0) {
        printf("Failed to initialize MMA8x5x sensor\n");
        return 1;
    }

    while (1) {
        if (mma8x5x_is_ready(&dev)) {
            int16_t x, y, z;
            mma8x5x_read(&dev, &x, &y, &z);
            printf("Acceleration X: %d, Y: %d, Z: %d\n", x, y, z);
        } else {
            printf("Sensor not ready\n");
        }

        xtimer_sleep(SLEEP_INTERVAL);
    }

    return 0;
}