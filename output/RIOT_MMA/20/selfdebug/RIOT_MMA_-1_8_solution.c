#include "periph_conf.h"
#include "periph/i2c.h"
#include "xtimer.h"

void mma8x5x_task(void) {
    i2c_t i2c_dev = I2C_DEV(0);
    uint8_t addr = CONFIG_MMA8X5X_I2C_ADDRESS; // Corrected macro name 
    mma8x5x_params_t params = {
        .i2c = i2c_dev,
        .addr = addr,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    mma8x5x_t dev;
    mma8x5x_data_t data;

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Initialization failed");
        return;
    }

    while (1) {
        if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
            printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        }
        else {
            puts("Read error");
        }
        xtimer_sleep(5);
    }
}

int main(void) {
    mma8x5x_task();
    return 0;
}