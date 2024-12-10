#include "xtimer.h"
#include "periph/gpio.h"
#include "kernel_defines.h"
#include "periph/i2c.h"

#include "mma8x5x.h"

static mma8x5x_t dev;
static mma8x5x_data_t data;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void read_accelerometer(void) {
    int16_t x, y, z;
    mma8x5x_read(&dev, &data);
    mma8x5x_get_accel(&data, &x, &y, &z);
    printf("Acceleration data - X: %d, Y: %d, Z: %d\n", x, y, z);
}

int main(void) {
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x\n");
        return 1;
    }
    
    while (1) {
        read_accelerometer();
        xtimer_sleep(5);
    }
    return 0;
}