#include "xtimer.h"
#include "periph/i2c.h"
#include "mma8x5x.h"
#include <stdio.h>

#define I2C_DEV_ID         (0)
#define MMA8X5X_ADDR       (0x1D)  // Default I2C address for MMA8653
#define READ_INTERVAL      (5U)    // Interval to read sensor data in seconds

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(I2C_DEV_ID),
    .addr = MMA8X5X_ADDR,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void read_sensor_data(void) {
    mma8x5x_data_t data;
    int res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x sensor\n");
        return;
    }

    while (1) {
        if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
            printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        } else {
            printf("Failed to read data\n");
        }
        xtimer_sleep(READ_INTERVAL);
    }
}

int main(void) {
    printf("Starting MMA8x5x sensor readings\n");
    read_sensor_data();
    return 0;
}