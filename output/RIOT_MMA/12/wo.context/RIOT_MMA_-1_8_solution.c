#include "xtimer.h"
#include "periph/i2c.h"

#define MMA8x5x_I2C_ADDRESS 0x1D
#define MMA8x5x_RATE_50HZ 50
#define MMA8x5x_RANGE_2G 2

static mma8x5x_t dev;
static mma8x5x_data_t data;

static const mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8x5x_I2C_ADDRESS,
    .rate = MMA8x5x_RATE_50HZ,
    .range = MMA8x5x_RANGE_2G
};

void read_sensor(void) {
    if (mma8x5x_read(&dev, &data) == MMA8x5x_OK) {
        printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
    } else {
        printf("Failed to read data\n");
    }
}

int main(void) {
    if (mma8x5x_init(&dev, &params) != MMA8x5x_OK) {
        printf("Failed to initialize MMA8x5x\n");
        return 1;
    }

    while (1) {
        read_sensor();
        xtimer_sleep(5);
    }

    return 0;
}