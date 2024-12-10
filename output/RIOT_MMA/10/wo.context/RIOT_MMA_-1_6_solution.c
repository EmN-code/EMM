#include <stdio.h>
#include "xtimer.h"
#include "periph/i2c.h"
#include "mma8x5x.h"

#define SLEEP_INTERVAL (5U)

static mma8x5x_t dev;

void read_sensor_data(void) {
    mma8x5x_data_t data;
    if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
        printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
    } else {
        puts("Error reading the sensor data");
    }
}

int main(void) {
    printf("Initializing I2C and MMA8x5x sensor\n");

    i2c_t i2c = I2C_DEV(0);
    mma8x5x_params_t params = {
        .i2c = i2c,
        .addr = 0x1D, // assuming default address
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    int res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("Failed to initialize accelerometer: %d\n", res);
        return 1;
    }

    while (1) {
        read_sensor_data();
        xtimer_sleep(SLEEP_INTERVAL);
    }

    return 0;
}