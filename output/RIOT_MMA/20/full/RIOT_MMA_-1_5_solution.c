#include "ztimer.h"
#include "timex.h"
#include "xtimer.h"
#include "stdio.h"
#include "stdlib.h"
#include "mma8x5x_params.h"
#include "mma8x5x.h"

#define SLEEP_INTERVAL (5U * 1000000U) // 5 seconds in microseconds

int main(void) {
    // Initialize MMA8x5x device
    mma8x5x_t dev;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };
    
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x\n");
        return 1;
    }

    while (1) {
        mma8x5x_data_t data;
        mma8x5x_read(&dev, &data);

        printf("Acceleration X: %d mg, Y: %d mg, Z: %d mg\n",
               data.x, data.y, data.z);

        ztimer_sleep(ZTIMER_SEC, 5); // Sleep for 5 seconds
    }

    return 0;
}