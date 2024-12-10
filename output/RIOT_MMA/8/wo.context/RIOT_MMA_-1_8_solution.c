#include <stdio.h>
#include <stdlib.h>
#include <periph/i2c.h>

#include "xtimer.h"
#include "mma8x5x.h"

#define DEVICE_I2C_ADDRESS    (0x1D)

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    printf("Initializing MMA8x5x accelerometer...\n");
    
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = DEVICE_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    int init_res = mma8x5x_init(&dev, &params);
    if (init_res != MMA8X5X_OK) {
        printf("Error initializing accelerometer: %d\n", init_res);
        return -1;
    }

    while (1) {
        xtimer_sleep(5);  // Sleep for 5 seconds
        
        mma8x5x_read(&dev, &data);
        printf("Acceleration [X:Y:Z] -> [%d:%d:%d]\n", data.x, data.y, data.z);
    }

    return 0;
}