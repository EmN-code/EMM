#include <xtimer.h>
#include <stdio.h>
#include <mma8x5x.h>

#define THREAD_STACKSIZE_DEFAULT (1024)

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    mma8x5x_t dev;

    int res = mma8x5x_init(&dev, &params);
    if (res == MMA8X5X_OK) {
        printf("MMA8x5x initialized successfully.\n");

        while (1) {
            if (mma8x5x_is_ready(&dev)) {
                mma8x5x_data_t data;
                mma8x5x_read(&dev, &data);
                printf("Acceleration X: %d, Y: %d, Z: %d\n", data.acc_x, data.acc_y, data.acc_z);
            } else {
                printf("MMA8x5x not ready.\n");
            }
            xtimer_sleep(5);
        }
    } else {
        printf("Failed to initialize MMA8x5x.\n");
    }

    return 0;
}