#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <periph/i2c.h>
#include <xtimer.h>
#include <mma8x5x.h>

#define I2C_ADDR        (0x1D)  // Example I2C address for the MMA8x5x device
#define I2C_BUS         (I2C_DEV(0))
#define SAMPLE_RATE     (5000000) // 5 seconds in microseconds

static mma8x5x_t dev;
static mma8x5x_params_t params = {
    .i2c = I2C_BUS,
    .addr = I2C_ADDR,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void read_mma8x5x(void) {
    mma8x5x_data_t data;
    if (mma8x5x_read(&dev, &data) == 0) {
        printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
    } else {
        puts("Error reading from MMA8x5x");
    }
}

int main(void) {
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Initialization failed!");
        return 1;
    }
    printf("MMA8x5x initialized
");

    while (1) {
        read_mma8x5x();
        xtimer_usleep(SAMPLE_RATE);
    }
    return 0;
}