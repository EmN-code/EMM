#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <periph/rtc.h>
#include <periph/i2c.h>
#include <xtimer.h>
#include "mma8x5x.h"

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void *read_sensor(void *arg) {
    (void) arg;
    mma8x5x_data_t data;
    while (1) {
        if (mma8x5x_read(&dev, &data) != 0) {
            printf("Error reading MMA8x5x\n");
            return NULL;
        }
        // Ensure correct format specifiers for int values
        printf("X: %d\tY: %d\tZ: %d\n", data.x, data.y, data.z);
        xtimer_sleep(5); // sleep for 5 seconds
    }
    return NULL;
}

int main(void) {
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x\n");
        return 1;
    }
    printf("MMA8x5x initialized successfully\n");
    read_sensor(NULL);
    return 0;
}