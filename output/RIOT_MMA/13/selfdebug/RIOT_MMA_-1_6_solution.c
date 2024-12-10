#include <ztimer.h>
#include <stdio.h>
#include <mma8x5x.h>
#include <msg.h>

static mma8x5x_t dev;

int main(void) {
    
    printf("MMA8x5x Accelerometer Example\n");

    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Initialization failed\n");
        return 1;
    }

    mma8x5x_data_t data;

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("X-axis: %d, Y-axis: %d, Z-axis: %d\n", data.x, data.y, data.z);
        ztimer_sleep(ZTIMER_SEC, 5);
    }
    return 0;
}