#include "board.h"
#include "xtimer.h"
#include "periph/i2c.h"
#include "mma8x5x.h"

#define I2C_DEV (I2C_DEV(0))  // Use I2C device 0

void init_i2c() {
    i2c_init(I2C_DEV, I2C_SPEED_FAST);
}

mma8x5x_t dev;

void init_mma8x5x() {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV,
        .addr = MMA8X5X_ADDR,
        .mode = MMA8X5X_MODE_ACTIVE,
    };

    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Failed to initialize MMA8x5x");
        while (1) {}
    }
}

void read_mma8x5x() {
    mma8x5x_data_t data;

    if (mma8x5x_read(&dev, &data) != MMA8X5X_OK) {
        puts("Failed to read from MMA8x5x");
        return;
    }

    printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
}

int main(void) {
    // Initialize the board
    board_init();

    // Initialize the I2C bus
    init_i2c();

    // Initialize the MMA8x5x sensor
    init_mma8x5x();

    // Main loop
    while (1) {
        read_mma8x5x();
        xtimer_sleep(5);  // Sleep for 5 seconds
    }

    return 0;
}