
#include "periph/i2c.h"
#include "xtimer.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"

#define SLEEP_INTERVAL (5 * 1000 * 1000U)  // 5 seconds in microseconds

int main(void) {
    mma8x5x_t dev;
    mma8x5x_data_t data;

    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    int res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        return 1;
    }

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration: X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);
        xtimer_usleep(SLEEP_INTERVAL);
    }

    return 0;
}
