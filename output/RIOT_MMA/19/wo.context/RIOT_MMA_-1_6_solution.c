#include "xtimer.h"
#include "mma8x5x.h"
#include "periph/i2c.h"
#include <stdio.h>

#define INTERVAL (5 * US_PER_SEC) // 5 seconds interval
#define SENSOR_ADDR CONFIG_MMA8X5X_I2C_ADDRESS // Corrected macro name

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = SENSOR_ADDR,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    int res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x: %d\n", res);
        return 1;
    }

    while(1) {
        xtimer_sleep(5); // Wait for 5 seconds

        res = mma8x5x_read(&dev, &data);
        if (res == MMA8X5X_OK) {
            printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        }
        else {
            printf("Failed to read MMA8x5x: %d\n", res);
        }
    }

    return 0;
}