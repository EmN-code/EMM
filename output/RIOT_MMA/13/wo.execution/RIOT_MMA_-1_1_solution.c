#include <xtimer.h>  // Include xtimer for timer functionalities.
#include <periph/i2c.h>  // Include I2C peripheral for sensor communication.
#include <mma8x5x.h>  // Include MMA8x5x sensor driver header.

// Main function to demonstrate reading from an MMA8x5x sensor every 5 seconds.
int main(void) {
    // Define sensor parameters.
    mma8x5x_params_t params = {
        .i2c = 0,  // Use I2C bus 0
        .addr = 0x1D,  // The default I2C address for MMA8x5x
    };
    
    // Initialize the MMA8x5x sensor device structure.
    mma8x5x_t dev;
    if (mma8x5x_init(&dev, &params) != 0) {
        // Print error and return if initialization fails.
        printf("Error initializing MMA8x5x sensor\n");
        return 1;
    }

    // Main loop to repeatedly read and output sensor data.
    while(1) {
        // Check if the sensor is ready for data reading.
        if (!mma8x5x_is_ready(&dev)) {
            // Print a warning and skip this cycle if the sensor is not ready.
            printf("Sensor not ready\n");
        } else {
            // Read sensor data.
            int16_t x, y, z;
            if (mma8x5x_read(&dev, &x, &y, &z) == 0) {
                // Print the read data.
                printf("Acceleration data: X: %d, Y: %d, Z: %d\n", x, y, z);
            } else {
                // Print an error if the data reading fails.
                printf("Error reading sensor data\n");
            }
        }

        // Wait for 5 seconds before reading the data again.
        xtimer_sleep(5);
    }

    // Return 0 in case the program exits successfully.
    return 0;
}