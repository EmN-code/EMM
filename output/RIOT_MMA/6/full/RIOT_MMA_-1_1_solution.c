#include "xtimer.h"
#include "mma8x5x.h"

int main(void) {
    mma8x5x_t dev;
    mma8x5x_params_t params;
    params.i2c = I2C_DEV(0);
    params.addr = MMA8X5X_DEFAULT_ADDR;

    // Initialize MMA8x5x sensor
    int err = mma8x5x_init(&dev, &params);
    while (err != 0) {
        printf("Failed to initialize MMA8x5x sensor\n");
        xtimer_sleep(1);
        err = mma8x5x_init(&dev, &params);
    }

    printf("MMA8x5x sensor initialized successfully\n");

    while (1) {
        // Check if the sensor is ready
        if (mma8x5x_is_ready(&dev)) {
            printf("Sensor is ready to read\n");

            // Read data from the sensor (mockup function, replace with actual API as needed)
            // mma8x5x_read(&dev, &acceleration_data);

            // Process and print the data
            // printf("Acceleration data: x=%d, y=%d, z=%d\n", acceleration_data.x, acceleration_data.y, acceleration_data.z);
        } else {
            printf("Sensor not ready, retrying...\n");
        }

        // Sleep for 5 seconds before the next read
        xtimer_sleep(5);
    }

    return 0;
}