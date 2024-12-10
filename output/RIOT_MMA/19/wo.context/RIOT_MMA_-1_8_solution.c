#include "xtimer.h"
#include "mma8x5x.h"
#include "i2c.h"
#include <stdio.h>

int main(void) {
    // Initialize I2C with the correct parameters
    i2c_t i2c = I2C_DEV(0);
    uint8_t mma8x5x_i2c_address = CONFIG_MMA8X5X_I2C_ADDRESS; // Corrected to use CONFIG_MMA8X5X_I2C_ADDRESS

    // Parameters for the MMA8x5x device
    mma8x5x_params_t params;
    params.i2c = i2c;
    params.addr = mma8x5x_i2c_address;
    params.rate = MMA8X5X_RATE_50HZ;
    params.range = MMA8X5X_RANGE_2G;

    mma8x5x_t dev;
    int result = mma8x5x_init(&dev, &params);
    if (result != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x sensor\n");
        return -1;
    }

    mma8x5x_data_t data;

    while (1) {
        // Read data from the sensor
        mma8x5x_read(&dev, &data);

        // Output the read values
        printf("X: %d mg, Y: %d mg, Z: %d mg\n", data.x, data.y, data.z);

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}