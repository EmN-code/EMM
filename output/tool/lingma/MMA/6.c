#include "board.h"
#include "xtimer.h"
#include "periph/i2c.h"
#include "mma8x5x.h"

static i2c_t i2c_dev;

void init_accelerometer(void) {
    // Initialize I2C bus
    i2c_init(&i2c_dev, I2C_DEV(0)); // Replace I2C_DEV(0) with your actual I2C device

    // Initialize MMA8x5x accelerometer
    mma8x5x_params_t params = {
        .i2c = &i2c_dev,
        .addr = MMA8X5X_ADDR,
        .mode = MMA8X5X_MODE_ACTIVE
    };

    if (mma8x5x_init(&params) != MMA8X5X_OK) {
        puts("Failed to initialize MMA8x5x");
        return;
    }

    puts("MMA8x5x initialized successfully");
}

void read_accelerometer_data(void) {
    mma8x5x_data_t data;

    if (mma8x5x_read_data(&data) != MMA8X5X_OK) {
        puts("Failed to read data from MMA8x5x");
        return;
    }

    printf("Accelerometer data: X=%d, Y=%d, Z=%d\n", data.x, data.y, data.z);
}

int main(void) {
    // Initialize the board
    board_init();

    // Initialize the accelerometer
    init_accelerometer();

    while (1) {
        // Read and print accelerometer data
        read_accelerometer_data();

        // Sleep for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}