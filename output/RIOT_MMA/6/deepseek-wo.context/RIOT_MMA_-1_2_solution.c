
#include "periph/i2c.h"
#include "mma8x5x.h"
#include "ztimer.h"
#include "fmt.h"

#define MMA8X5X_I2C_ADDRESS CONFIG_MMA8X5X_I2C_ADDRESS
#define MMA8X5X_RATE MMA8X5X_RATE_50HZ

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void)
{
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE,
        .range = MMA8X5X_RANGE_2G
    };

    int res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x accelerometer\n");
        return 1;
    }

    while (1) {
        mma8x5x_read(&dev, &data);
        printf("Acceleration: X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);
        ztimer_sleep(ZTIMER_MSEC, 5000);
    }

    return 0;
}
