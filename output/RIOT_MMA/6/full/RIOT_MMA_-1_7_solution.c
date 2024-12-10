// This is a code solution to read data from an MMA8x5x sensor using an ESP32, and log the readings every 5 seconds.

#include <xtimer.h>
#include <stdio.h>
#include <stdlib.h>
#include <mma8x5x.h>
#include <i2c.h>

// MMA8x5x sensor initialization parameters
static mma8x5x_t dev;
static mma8x5x_data_t data;
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

void *read_sensor(void *arg) {
    (void)arg;  // To avoid unused parameter warning
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x\n");
        return NULL;
    }

    while (1) {
        // Read data from the sensor
        mma8x5x_read(&dev, &data);
        
        // Log the accelerometer data
        printf("Acceleration X: %d mg\n", data.x);
        printf("Acceleration Y: %d mg\n", data.y);
        printf("Acceleration Z: %d mg\n", data.z);
        
        // Wait for 5 seconds
        xtimer_sleep(5);
    }
}

int main(void) {
    // Create a thread to read from the sensor
    char stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, read_sensor, NULL, "read_sensor");

    return 0;
}