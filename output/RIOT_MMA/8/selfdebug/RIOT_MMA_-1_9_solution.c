#include "periph/i2c.h"
#include "msg.h"
#include "mma8x5x.h"

static mma8x5x_t dev;

void read_mma8x5x(void) {
    mma8x5x_data_t data;
    if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
        printf("MMA8x5x Data - X: %d, Y: %d, Z: %d\n",
               data.raw[0], data.raw[1], data.raw[2]);
    }
    else {
        printf("Failed to read MMA8x5x data\n");
    }
}

int main(void) {
    msg_t msg;

    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x\n");
        return -1;
    }

    while (1) {
        read_mma8x5x();
        xtimer_sleep(5);
    }

    return 0;
}