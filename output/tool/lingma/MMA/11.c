#include "board.h"
#include "xtimer.h"
#include "periph/i2c.h"
#include "mma8x5x.h"

#define I2C_DEV (I2C_DEV(0))  // Use I2C device 0

void init_i2c(void) {
    i2c_init(I2C_DEV, I2C_SPEED_FAST);
}

mma8x5x_t dev;

void init_mma8x5x(void) {
    if (mma8x5x_init(&dev, I2C_DEV, MMA8X5X_ADDR) != MMA8X5X_OK) {
        puts("Failed to initialize MMA8x5x");
        while (1) {}
    }
}

void read_mma8x5x(void) {
    int16_t x, y, z;
    if (mma8x5x_read_xyz(&dev, &x, &y, &z) == MMA8X5X_OK) {
        printf("X: %d, Y: %d, Z: %d\n", x, y, z);
    } else {
        puts("Failed to read from MMA8x5x");
    }
}

int main(void) {
    // Initialize the board
    board_init();

    // Initialize I2C
    init_i2c();

    // Initialize MMA8x5x
    init_mma8x5x();

    // Main loop
    while (1) {
        read_mma8x5x();
        xtimer_sleep(5);  // Sleep for 5 seconds
    }

    return 0;
}