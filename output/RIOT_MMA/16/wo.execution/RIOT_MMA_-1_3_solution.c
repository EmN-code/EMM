#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "periph_conf.h"
#include "periph/i2c.h"
#include "periph/gpio.h"
#include "periph/rtc.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "event/thread.h"
#include "thread.h"
#include <stdio.h>

#define SLEEP_INTERVAL_S (5U)

/* Define the I2C device and the address for the MMA8x5x */
#define I2C_DEV        I2C_DEV(0)
#define MMA8X5X_I2C_ADDRESS  (0x1D) // Example I2C address; verify with sensor specs.

int main(void) {
    /* Set up the device descriptor and parameters */
    mma8x5x_t dev;
    mma8x5x_data_t data;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV,
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    /* Initialize the device */
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Error initializing MMA8x5x device\n");
        return -1;
    }

    while (1) {
        /* Check if the sensor is ready */
        if (mma8x5x_is_ready(&dev)) {
            /* Read accelerometer data */
            mma8x5x_read(&dev, &data);
            printf("Acceleration: x=%dmg y=%dmg z=%dmg\n", data.x, data.y, data.z);
        } else {
            printf("MMA8x5x not ready\n");
        }

        /* Wait for 5 seconds */
        ztimer_sleep(ZTIMER_SEC, SLEEP_INTERVAL_S);
    }

    return 0;
}