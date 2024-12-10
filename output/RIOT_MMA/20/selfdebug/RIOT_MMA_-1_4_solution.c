#include "mma8x5x.h"
#include "periph/i2c.h"
#include "kernel.h"

#define I2C_DEV 0
#define SLEEP_INTERVAL (5U * US_PER_SEC)
#define SENSOR_ADDR 0x1D // Default I2C address for MMA8x5x

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV,
    .addr = SENSOR_ADDR,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

int main(void) {
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Initialization failed\n");
        return 1;
    }
    
    while (1) {
        mma8x5x_data_t data;
        mma8x5x_read(&dev, &data);
        printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        xtimer_usleep(SLEEP_INTERVAL);
    }

    return 0;
}