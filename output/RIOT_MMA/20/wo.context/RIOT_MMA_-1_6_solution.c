#include "xtimer.h"
#include "periph/i2c.h"
#include "mma8x5x.h"

#define I2C_INTERFACE I2C_DEV(0)
#define ADDR         (0x1D)           // Default I2C address for MMA8653
#define INTERVAL     (5U * 1000000U)  // 5 seconds in microseconds

static mma8x5x_t dev;
static mma8x5x_data_t data;
static mma8x5x_params_t params = {
    .i2c = I2C_INTERFACE,
    .addr = ADDR,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

int main(void) {
    printf("MMA8x5x accelerometer test application
");

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Initialization failed\n");
        return -1;
    }

    while (1) {
        if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
            printf("X: %d   Y: %d   Z: %d
", data.x, data.y, data.z);
        } else {
            printf("Failed to read data\n");
        }
        xtimer_usleep(INTERVAL);
    }

    return 0;
}