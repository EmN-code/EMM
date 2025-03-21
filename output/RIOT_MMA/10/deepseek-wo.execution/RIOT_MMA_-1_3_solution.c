
#include <stdio.h>
#include <ztimer.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>

#define SLEEP_INTERVAL (5 * ZTIMER_SEC)

static mma8x5x_t dev;
static mma8x5x_data_t data;

void read_accelerometer(void) {
    mma8x5x_read(&dev, &data);
    printf("Acceleration: X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);
}

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x accelerometer\n");
        return 1;
    }

    while (1) {
        read_accelerometer();
        ztimer_sleep(ZTIMER_SEC, SLEEP_INTERVAL);
    }

    return 0;
}
