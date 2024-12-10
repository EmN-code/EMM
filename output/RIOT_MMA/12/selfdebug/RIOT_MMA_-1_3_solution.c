#include <xtimer.h>
#include <stdio.h>
#include <periph/i2c.h>

#define I2C_DEV 0
#define MMA8X5X_I2C_ADDRESS 0x1D

typedef struct {
    i2c_t i2c;
    uint8_t addr;
    uint8_t rate;
    uint8_t range;
} mma8x5x_params_t;

typedef struct {
    mma8x5x_params_t params;
} mma8x5x_t;

void mma8x5x_read_data(mma8x5x_t *dev) {
    // Assume a dummy function call to read data from the sensor
    printf("Reading data from MMA8x5x sensor...\n");
}

int main(void) {
    mma8x5x_t dev;
    mma8x5x_params_t params;

    params.i2c = I2C_DEV;
    params.addr = MMA8X5X_I2C_ADDRESS;
    params.rate = 50;  // Assuming 50Hz as per the documentation
    params.range = 2;  // Assuming 2G as per the documentation

    dev.params = params;

    while (1) {
        mma8x5x_read_data(&dev);
        xtimer_sleep(5);  // Sleep for 5 seconds
    }

    return 0;
}