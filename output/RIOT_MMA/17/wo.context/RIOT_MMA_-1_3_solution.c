#include <xtimer.h>
#include <periph/i2c.h>
#include <mma8x5x.h>
#include <stdio.h>

#define I2C_INTERFACE    I2C_DEV(0)   // Use a valid I2C device
#define MMA8X5X_ADDR     (0x1D)       // Use correct constant for the address

static mma8x5x_t dev;

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_INTERFACE,
        .addr = MMA8X5X_ADDR,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Initialization failed.\n");
        return 1;
    }

    while (1) {
        mma8x5x_data_t data;
        if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
            printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        } else {
            printf("Read failed.\n");
        }

        xtimer_sleep(5); // sleep for 5 seconds
    }

    return 0;
}