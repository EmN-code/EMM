#include "board.h"
#include "xtimer.h"
#include "periph/i2c.h"
#include "mma8x5x.h"

#define I2C_DEV (I2C_DEV(0)) // Use I2C device 0

void i2c_init(void) {
    i2c_init(I2C_DEV, I2C_SPEED_FAST);
}

mma8x5x_t dev;

void mma8x5x_init(void) {
    if (mma8x5x_init(&dev, I2C_DEV) != MMA8X5X_OK) {
        puts("Failed to initialize MMA8x5x");
        return;
    }

    if (mma8x5x_start_measurement(&dev) != MMA8X5X_OK) {
        puts("Failed to start measurement");
        return;
    }
}

void read_mma8x5x(void) {
    mma8x5x_data_t data;

    if (mma8x5x_read_data(&dev, &data) == MMA8X5X_OK) {
        printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
    } else {
        puts("Failed to read data from MMA8x5x");
    }
}

int main(void) {
    i2c_init();
    mma8x5x_init();

    while (1) {
        read_mma8x5x();
        xtimer_sleep(5); // Sleep for 5 seconds
    }

    return 0;
}