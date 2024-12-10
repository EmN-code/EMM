#include "xtimer.h"
#include "timex.h"
#include "mma8x5x.h"
#include "periph/i2c"  
#include <stdio.h>

#define MMA8X5X_DEFAULT_I2C I2C_DEV(0)
#define MMA8X5X_DEFAULT_ADDR 0x1D
#define INTERVAL (5U * US_PER_SEC)  // 5 seconds interval

int main(void)
{
    mma8x5x_t dev;
    mma8x5x_params_t mma8x5x_params = {
        .i2c = MMA8X5X_DEFAULT_I2C,
        .addr = MMA8X5X_DEFAULT_ADDR
    };

    /* initialize the sensor */
    if (mma8x5x_init(&dev, &mma8x5x_params) != 0) {
        puts("Initialization failed");
        return 1;
    }

    puts("MMA8x5x sensor initialized");

    while (1) {
        int16_t acc[3];

        /* read acceleration data */
        if (mma8x5x_read(&dev, acc) != 0) {
            puts("Failed to read data");
        } else {
            printf("Acceleration: x=%d y=%d z=%d\n", acc[0], acc[1], acc[2]);
        }

        /* wait 5 seconds */
        xtimer_sleep(INTERVAL);
    }

    return 0;
}