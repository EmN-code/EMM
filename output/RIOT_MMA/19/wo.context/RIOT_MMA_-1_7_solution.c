#include "xtimer.h"
#include "periph/i2c.h"
#include "mma8x5x.h"

#define SLEEP_DURATION (5U * 1000000U)  // 5 seconds in microseconds

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    // Define parameters for the MMA8x5x device
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,  // Ensure correct I2C address constant is used
        .rate = MMA8X5X_RATE_50HZ,    // Ensure to use the correct predefined rate
        .range = MMA8X5X_RANGE_2G     // Ensure to use the correct predefined range
    };

    // Initialize the MMA8x5x device
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Initialization failed");
        return 1;
    }

    puts("Initialization successful");
    
    while (1) {
        // Read data from the MMA8x5x device
        if (mma8x5x_read(&dev, &data) != MMA8X5X_OK) {
            puts("Read failed");
        } else {
            printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        }

        // Sleep for 5 seconds before next read
        xtimer_sleep(5);
    }
    return 0;
}
