#include "periph/i2c.h"
#include "stdio.h"
#include "stdint.h"
#include "xtimer.h"
#include "mma8x5x.h"

#define SLEEP_INTERVAL    (5U * US_PER_SEC)  // 5 seconds

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    puts("MMA8x5x sensor test application");

    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Error: Could not initialize MMA8x5x sensor");
        return 1;
    }

    while (1) {
        if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
            printf("Accelerometer: x = %d, y = %d, z = %d\n",
                   data.x, data.y, data.z);
        } else {
            puts("Error: Failed to read sensor data");
        }

        xtimer_sleep(SLEEP_INTERVAL);
    }

    return 0;
}