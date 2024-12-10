#include <ztimer.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <pthread.h>
#include <timex.h>
#include <unistd.h>
#include <mma8x5x.h>
#include <mma8x5x_params.h>

#define I2C_DEV(0) 0  // Mock I2C device for demonstration
#define MMA8X5X_I2C_ADDRESS 0x1D  // Default I2C address for MMA8x5x series


static mma8x5x_t dev;
mma8x5x_data_t data;

void *read_accel_data(void *arg) {
    // Initialize the sensor
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };
    int res = mma8x5x_init(&dev, &params);
    if (res != MMA8X5X_OK) {
        printf("Error initializing MMA8x5x\n");
        return NULL;
    }

    while (1) {
        // Read acceleration data if device is ready
        if (mma8x5x_is_ready(&dev)) {
            mma8x5x_read(&dev, &data);
            printf("Acceleration X: %d mg\n", data.x_axis);
            printf("Acceleration Y: %d mg\n", data.y_axis);
            printf("Acceleration Z: %d mg\n", data.z_axis);
        }
        // Sleep for 5 seconds
        sleep(5);
    }
    return NULL;
}

int main(void) {
    pthread_t thread;

    // Create a separate thread to read accelerometer data
    pthread_create(&thread, NULL, read_accel_data, NULL);

    // Prevent main from exiting
    pthread_join(thread, NULL);

    return 0;
}