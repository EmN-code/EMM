#include "xtimer.h"
#include "periph/i2c.h"
#include "mma8x5x.h"

#define I2C_CHANNEL        I2C_DEV(0)
#define I2C_ADDR           MMA8X5X_I2C_ADDRESS
#define READ_INTERVAL      (5U * US_PER_SEC) // 5 seconds

static mma8x5x_t dev;

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_CHANNEL,
        .addr = I2C_ADDR,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    int res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x\n");
        return 1;
    }

    while (1) {
        mma8x5x_data_t data;
        mma8x5x_read(&dev, &data);
        printf("Acceleration [X Y Z]: %d %d %d\n", data.x, data.y, data.z);
        xtimer_sleep(5);
    }

    return 0;
}