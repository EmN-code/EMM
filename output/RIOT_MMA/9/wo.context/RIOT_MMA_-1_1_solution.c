#include <thread.h>
#include <stdio.h>
#include <string.h>
#include <mma8x5x.h>
#include <periph_conf.h>
#include <xtimer.h>

// Parameters for initializing the MMA8x5x sensor
static mma8x5x_params_t mma8x5x_params = {
    // Set the correct I2C interface and slave address
    .i2c = I2C_DEV(0),  // Adjust based on your setup
    .addr = MMA8X5X_ADDR(0),  // Use proper address
};

static mma8x5x_t dev;

// Function to read data from the sensor
void read_sensor(void) {
    int16_t acc[3];

    // First, check if the sensor is ready
    if (mma8x5x_is_ready(&dev)) {
        // Read the acceleration data
        if (mma8x5x_read(&dev, acc) == 0) {
            printf("Acceleration x: %d, y: %d, z: %d\n", acc[0], acc[1], acc[2]);
        } else {
            printf("Error reading from MMA8x5x\n");
        }
    } else {
        printf("MMA8x5x sensor is not ready\n");
    }
}

int main(void) {
    // Initialize the MMA8x5x sensor
    if (mma8x5x_init(&dev, &mma8x5x_params) != 0) {
        printf("Failed to initialize MMA8x5x\n");
        return -1;
    }

    printf("MMA8x5x sensor initialized.\n");

    // Run the read_sensor function every 5 seconds
    while (1) {
        read_sensor();
        xtimer_sleep(5);
    }

    return 0;
}