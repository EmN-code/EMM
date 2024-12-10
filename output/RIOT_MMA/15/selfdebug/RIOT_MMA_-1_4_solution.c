#include "xtimer.h"
#include "timex.h"
#include "periph/i2c.h"
#include "periph/gpio.h"

#include "mma8x5x.h"  // Ensure this file is included for mma8x5x types and functions

#define I2C_DEV       I2C_DEV(0)  // Default I2C device
#define READ_INTERVAL (5U * US_PER_SEC)  // 5 seconds

static mma8x5x_t dev;

static mma8x5x_params_t params = {
    .i2c = I2C_DEV,
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

static void read_mma8x5x(void)
{
    mma8x5x_data_t data;
    if (mma8x5x_read(&dev, &data) == 0) {
        printf("Acceleration [mg]: X = %d, Y = %d, Z = %d\n", data.x_axis, data.y_axis, data.z_axis);
    } else {
        puts("Failed to read data from the accelerometer");
    }
}

int main(void)
{
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Failed to initialize accelerometer");
        return 1;
    }

    ztimer_sleep(ZTIMER_SEC, 1);
    while (1) {
        read_mma8x5x();
        ztimer_sleep(ZTIMER_USEC, READ_INTERVAL);
    }

    return 0;
}