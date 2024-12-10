#include "xtimer.h"
#include "periph/i2c.h"
#include "mma8x5x.h"

#define SLEEP_INTERVAL (5U * US_PER_SEC) // 5 seconds

int main(void) {
    // Initialize the parameters for the MMA8x5x device
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    mma8x5x_t dev;

    // Initialize the MMA8x5x device
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        // Initialization failed, handle the error
        return 1;
    }

    mma8x5x_data_t data;

    while (1) {
        // Read data from the MMA8x5x sensor
        if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
            // Successfully read the sensor data
            // Process the data (e.g., print the values)
            printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        }
        else {
            // Reading data failed, handle the error
            printf("Failed to read data from MMA8x5x\n");
        }

        // Sleep for the defined interval
        xtimer_sleep(SLEEP_INTERVAL / US_PER_SEC);
    }

    return 0;
}