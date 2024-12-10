#include <xtimer.h> // Import for xtimer_sleep 
#include <stdio.h> // Import for printf and puts
#include "mma8x5x.h" // MMA8x5x device-specific includes
#include "periph/i2c.h" // I2C peripheral includes

static mma8x5x_t dev;
static mma8x5x_data_t data;

// Define the parameters for the MMA8x5x device
static mma8x5x_params_t params = {
    .i2c = I2C_DEV(0),
    .addr = MMA8X5X_I2C_ADDRESS,
    .rate = MMA8X5X_RATE_50HZ,
    .range = MMA8X5X_RANGE_2G
};

int main(void) {
    // Initialize the device
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Initialization error.");
        return 1;
    }

    // Infinitely read data from the sensor every 5 seconds
    while (1) {
        // Read data from the MMA8x5x sensor
        mma8x5x_read(&dev, &data);
        
        // Output the sensor data
        printf("Accelerometer data: X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
        
        // Sleep for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}