#include "xtimer.h"
#include "periph/i2c.h"
#include "mma8x5x.h"

#define I2C_DEVICE       (I2C_DEV(0))
#define MMA8X5X_I2C_ADDR (0x1D)
#define READ_INTERVAL    (5U * US_PER_SEC)

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_DEVICE,
    .addr = MMA8X5X_I2C_ADDR,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

int main(void)
{
    int res;
    mma8x5x_data_t data;

    res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("Initialization failed\n");
        return 1;
    }

    while (1) {
        xtimer_sleep(READ_INTERVAL / US_PER_SEC);

        if (mma8x5x_read(&dev, &data) != MMA8X5X_OK) {
            printf("Failed to read data\n");
        } else {
            printf("Acceleration - X: %d, Y: %d, Z: %d\n", data.x_axis, data.y_axis, data.z_axis);
        }
    }

    return 0;
}