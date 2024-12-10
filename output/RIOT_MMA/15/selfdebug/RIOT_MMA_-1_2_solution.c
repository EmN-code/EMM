#include "periph/i2c.h"
#include "mma8x5x.h"
#include "kernel_defines.h"
#include <stdio.h>
#include <string.h>
#include <xtimer.h>

#define I2C_DEV I2C_DEV(0)
#define MMA8X5X_ADDR 0x1D  // typical address for MMA8x5x
#define READ_INTERVAL (5 * US_PER_SEC)  // 5 seconds

int main(void) {
    // Define parameters for MMA8x5x
    mma8x5x_params_t params = {
        .i2c = I2C_DEV,
        .addr = MMA8X5X_ADDR,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };
    
    mma8x5x_t dev;
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Initialization failed\n");
        return 1;
    }
    printf("MMA8x5x initialized\n");

    mma8x5x_data_t data;
    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration: X=%d, Y=%d, Z=%d\n", data.x, data.y, data.z);
        xtimer_sleep(5);  // sleep for 5 seconds
    }
    return 0;
}