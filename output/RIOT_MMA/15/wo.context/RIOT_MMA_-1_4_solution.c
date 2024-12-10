#include "xtimer.h"
#include "periph/i2c.h"
#include "mma8x5x.h"

#define SLEEP_DURATION (5U * US_PER_SEC)

static mma8x5x_t dev;
static mma8x5x_data_t data;

int main(void) {
    // Set up the device parameters
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = CONFIG_MMA8X5X_I2C_ADDRESS,  // Correctly declare the address
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    // Initialize the MMA8x5x sensor
    while (1) {
        int rc = mma8x5x_init(&dev, &params);
        if (rc != MMA8X5X_OK) {
            printf("Initialization failed with code %d\n", rc);
            xtimer_sleep(1); // Retry initialization every second
            continue;
        } else {
            printf("MMA8x5x successfully initialized.\n");
            break;
        }
    }

    // Reading loop
    while (1) {
        // Read sensor data
        mma8x5x_read(&dev, &data);
        printf("Acceleration in mg - X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);

        // Sleep for 5 seconds
        xtimer_sleep(SLEEP_DURATION);
    }

    return 0;
}