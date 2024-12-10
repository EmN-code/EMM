#include "board.h"
#include "xtimer.h"
#include "periph/i2c.h"
#include "mma8x5x.h"

#define I2C_DEV (I2C_DEV(0)) // Use I2C device 0
#define I2C_SPEED (100000U)  // Set I2C speed to 100 kHz

mma8x5x_t dev;

void init_i2c(void) {
    i2c_init(I2C_DEV, I2C_SPEED);
}

void init_mma8x5x(void) {
    if (mma8x5x_init(&dev, I2C_DEV) != MMA8X5X_OK) {
        puts("Failed to initialize MMA8x5x");
        return;
    }

    if (mma8x5x_power_on(&dev) != MMA8X5X_OK) {
        puts("Failed to power on MMA8x5x");
        return;
    }

    if (mma8x5x_set_odr(&dev, MMA8X5X_ODR_100) != MMA8X5X_OK) {
        puts("Failed to set ODR");
        return;
    }
}

void read_mma8x5x(void) {
    mma8x5x_data_t data;

    if (mma8x5x_read_data(&dev, &data) != MMA8X5X_OK) {
        puts("Failed to read data from MMA8x5x");
        return;
    }

    printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
}

int main(void) {
    init_i2c();
    init_mma8x5x();

    while (1) {
        read_mma8x5x();
        xtimer_sleep(5); // Sleep for 5 seconds
    }

    return 0;
}