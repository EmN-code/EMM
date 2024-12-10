#include "xtimer.h"
#include "periph/i2c.h"
#include "mma8x5x.h"

#define I2C_ADDRESS (0x1D)
#define I2C_RATE (MMA8X5X_RATE_50HZ)
#define I2C_RANGE (MMA8X5X_RANGE_2G)
#define SLEEP_INTERVAL (5U * US_PER_SEC)

static mma8x5x_t dev;

int main(void) {
    mma8x5x_params_t params;
    params.i2c = I2C_DEV(0);  // Correctly define I2C device
    params.addr = I2C_ADDRESS;
    params.rate = I2C_RATE;
    params.range = I2C_RANGE;

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Initialization of MMA8x5x failed!");
        return 1;
    }

    mma8x5x_data_t data;

    while (1) {
        xtimer_sleep(SLEEP_INTERVAL);

        if (mma8x5x_read(&dev, &data) != 0) {
            puts("Error reading sensor data!");
        } else {
            printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        }
    }
    return 0;
}