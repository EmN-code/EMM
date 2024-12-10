#include "board.h"
#include "xtimer.h"
#include "mma8x5x.h"

int main(void)
{
    // Initialize the board
    board_init();

    // Initialize the MMA8x5x sensor
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),  // Use I2C device 0
        .addr = MMA8X5X_I2C_ADDRESS,  // Default I2C address
    };

    mma8x5x_t dev;
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Failed to initialize MMA8x5x");
        return 1;
    }

    puts("MMA8x5x initialized successfully");

    // Main loop
    while (1) {
        // Read data from the sensor
        mma8x5x_data_t data;
        if (mma8x5x_read_data(&dev, &data) != MMA8X5X_OK) {
            puts("Failed to read data from MMA8x5x");
            continue;
        }

        // Print the data
        printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}