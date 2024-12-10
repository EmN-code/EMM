#include <xtimer.h>
#include <periph/i2c.h>
#include <mma8x5x.h>

#define MMA8X5X_I2C_ADDRESS CONFIG_MMA8X5X_I2C_ADDRESS
#define INTERVAL (5U * US_PER_SEC)

static mma8x5x_t dev;

int main(void)
{
    printf("Starting MMA8X5x accelerometer reading example\n");

    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Initialization failed\n");
        return -1;
    }

    while (1) {
        int16_t data[3];
        mma8x5x_read(&dev, data);
        printf("x-axis: %d, y-axis: %d, z-axis: %d\n", data[0], data[1], data[2]);
        xtimer_sleep(INTERVAL);
    }
    return 0;
}
