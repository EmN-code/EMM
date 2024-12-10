#include "xtimer.h"
#include "mma8x5x.h"
#include "stdio.h"

// Constants and parameters for I2C and the MMA8x5x sensor
i2c_t i2c_dev = I2C_DEV(0);
const uint8_t MMA8X5X_I2C_ADDRESS = 0x1D;
const uint8_t RATE = MMA8X5X_RATE_50HZ;
const uint8_t RANGE = MMA8X5X_RANGE_2G;

mma8x5x_t dev;

int main(void) {
    // Parameters for the accelerometer
    mma8x5x_params_t params = {
        .i2c = i2c_dev,
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = RATE,
        .range = RANGE
    };
    
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize accelerometer\n");
        return 1;
    }

    mma8x5x_data_t data;

    while (1) {
        // Read the accelerometer data
        mma8x5x_read(&dev, &data);

        // Display the data
        printf("Acceleration X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}