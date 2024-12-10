#include "mma8x5x.h"
#include "periph/gpio.h"
#include "periph/i2c.h"
#include "xtimer.h"

#define MMA8X5X_I2C_ADDRESS      (0x1D)  // Replace with the correct I2C address if necessary
#define SLEEP_INTERVAL           (5 * US_PER_SEC)  // 5 seconds to microseconds

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    // Initialize parameters for the MMA8x5x device
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    // Initialize the accelerometer device
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Error: Unable to initialize the MMA8x5x device\n");
        return -1;
    }
    
    while (1) {
        xtimer_sleep(SLEEP_INTERVAL / US_PER_SEC);  // Sleep for 5 seconds

        // Read data from the accelerometer
        mma8x5x_read(&dev, &data);

        // Print the accelerometer data
        printf("Acceleration: X=%d mg, Y=%d mg, Z=%d mg\n", data.x, data.y, data.z);
    }

    return 0;
}