#include <stdio.h>
#include "thread.h"
#include "periph/i2c.h"

#define SLEEP_INTERVAL (5U)

static mma8x5x_t dev;
static mma8x5x_data_t data;
static const mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void initialize_sensor(void) {
    int res = mma8x5x_init(&dev, &params);
    if(res != MMA8X5X_OK) {
        printf("Initialization failed\n");
    }
}

void read_accelerometer(void) {
    mma8x5x_read(&dev, &data);
    printf("Accelerometer data: x: %d, y: %d, z: %d\n",
            data.x, data.y, data.z);
}

int main(void) {
    initialize_sensor();
    while (1) {
        read_accelerometer();
        xtimer_sleep(SLEEP_INTERVAL);
    }
    return 0;
}