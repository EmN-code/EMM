#include <xtimer.h>
#include <stdio.h>
#include <periph/i2c.h>
#include "mma8x5x.h"

#define SLEEP_INTERVAL (5U * US_PER_SEC)  // 5 seconds sleep interval

typedef struct {
    i2c_t i2c;
    uint8_t addr;
    uint8_t rate;
    uint8_t range;
    uint8_t offset[3];
} mma8x5x_params_t;

int main(void) {
    mma8x5x_t dev;
    mma8x5x_data_t data;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G,
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Initialization failed\n");
        return 1;
    }

    while (1) {
        if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
            printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        } else {
            printf("Failed to read data\n");
        }
        xtimer_sleep(SLEEP_INTERVAL);
    }

    return 0;
}