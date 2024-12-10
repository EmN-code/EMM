#include <xtimer.h>  // For using timers
#include "mma8x5x_params_t.h"  // Assuming your project has these files
#include "mma8x5x.h"  // Adjust based on your project

#define WAIT_TIME (5U * US_PER_SEC)  // 5 seconds delay defined as microseconds

int main(void) {
    // Initialize the device descriptor and parameters for the sensor
    mma8x5x_t dev;
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };

    // Initialize the sensor
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Initialization failed.\n");
        return 1;
    }

    while (1) {
        // Delay for 5 seconds
        xtimer_sleep(WAIT_TIME);

        // Fetch data
        mma8x5x_data_t data;
        if (mma8x5x_read(&dev, &data) != MMA8X5X_OK) {
            printf("Error reading data.\n");
            continue;
        }

        // Display the data
        printf("Acceleration data: X=%d, Y=%d, Z=%d\n", data.x, data.y, data.z);
    }

    return 0;  // Though this line will never be reached
}