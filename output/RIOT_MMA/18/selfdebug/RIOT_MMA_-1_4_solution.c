#include "xtimer.h"
#include "periph/i2c.h"
#include "mma8x5x.h"
#include <stdio.h>

#define I2C_DEVICE      I2C_DEV(0)
#define I2C_ADDRESS     0x1D  // Adjust based on the actual device
#define SLEEP_DURATION  (5U * US_PER_SEC)

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_DEVICE,
    .addr = I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void read_accelerometer(void) {
    mma8x5x_data_t data;
    mma8x5x_read(&dev, &data);
    printf("Acceleration [mg]: X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
}

int main(void) {
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("MMA8x5x initialization failed\n");
        return 1;
    }
    printf("MMA8x5x initialized\n");

    while (1) {
        read_accelerometer();
        xtimer_sleep(SLEEP_DURATION);
    }
    return 0;
}