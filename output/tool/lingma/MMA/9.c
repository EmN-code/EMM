#include "periph/i2c.h"
#include "xtimer.h"
#include "mma8x5x.h"

#define I2C_DEV I2C_DEV(0) // Use I2C device 0
#define I2C_SPEED I2C_SPEED_FAST // Set I2C speed to fast mode (400 kHz)

int main(void) {
    // Initialize the I2C bus
    i2c_init(I2C_DEV, I2C_SPEED);

    // Initialize the MMA8x5x sensor
    mma8x5x_init(I2C_DEV, MMA8X5X_ADDR);

    // Main loop
    while (1) {
        // Read data from the MMA8x5x
        mma8x5x_data_t data;
        mma8x5x_read_data(&data);

        // Print the data
        printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}