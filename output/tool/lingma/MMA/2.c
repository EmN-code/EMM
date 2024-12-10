#include "board.h"
#include "xtimer.h"
#include "periph/i2c.h"
#include "mma8x5x.h"

#define I2C_DEV (I2C_DEV(0)) // Use I2C device 0

void i2c_init(void) {
    i2c_init_dev(I2C_DEV);
    i2c_acquire(I2C_DEV);
}

mma8x5x_t dev;

void mma8x5x_init(void) {
    if (mma8x5x_init(&dev, I2C_DEV, MMA8X5X_ADDR) != MMA8X5X_OK) {
        puts("Failed to initialize MMA8x5x");
        return;
    }
    puts("MMA8x5x initialized successfully");
}

void read_mma8x5x_data(void) {
    int16_t x, y, z;
    if (mma8x5x_read_xyz(&dev, &x, &y, &z) != MMA8X5X_OK) {
        puts("Failed to read data from MMA8x5x");
        return;
    }
    printf("X: %d, Y: %d, Z: %d\n", x, y, z);
}

int main(void) {
    // Initialize the board
    board_init();

    // Initialize the I2C bus
    i2c_init();

    // Initialize the MMA8x5x sensor
    mma8x5x_init();

    while (1) {
        // Read data from the MMA8x5x sensor
        read_mma8x5x_data();

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}