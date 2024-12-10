#include "xtimer.h"
#include "periph/i2c.h"
#include "mma8x5x.h"

#define I2C_BUS 0
#define ADDR MMA8X5X_I2C_ADDRESS
#define READ_INTERVAL (5 * US_PER_SEC)  // 5 seconds in microseconds

int main(void) {
    // Initialize parameters for the MMA8x5x device
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(I2C_BUS),
        .addr = ADDR,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    // Device descriptor
    mma8x5x_t dev;

    // Initialize the MMA8x5x sensor
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x\n");
        return 1;
    }

    while (1) {
        // Data structure for storing the acceleration data
        mma8x5x_data_t data;
        
        // Read acceleration data from the sensor
        mma8x5x_read(&dev, &data);
        printf("Acceleration - X: %d mg, Y: %d mg, Z: %d mg\n",
               data.x, data.y, data.z);

        // Wait for the next read interval
        xtimer_sleep(READ_INTERVAL);
    }

    return 0;
}