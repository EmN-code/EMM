#include <stdio.h> // Standard Input/Output library
#include <stdlib.h> // Standard Library
#include "periph/i2c.h" // Peripheral I2C
#include "mma8x5x.h" // MMA8x5x driver
#include "xtimer.h" // Timer for periodic execution

#define MMA8X5X_I2C_ADDRESS CONFIG_MMA8X5X_I2C_ADDRESS // Correct I2C address definition
#define INTERVAL (5U * US_PER_SEC) // 5 seconds interval

static mma8x5x_t dev; // Device descriptor
static mma8x5x_data_t data; // Data struct to store readings

void read_sensor(void) {
    if (mma8x5x_read(&dev, &data) == MMA8X5X_OK) {
        printf("X: %d\tY: %d\tZ: %d\n", data.x, data.y, data.z);
    } else {
        puts("Error reading from sensor");
    }
}

int main(void) {
    mma8x5x_params_t params = {
        .i2c = I2C_DEV(0),
        .addr = MMA8X5X_I2C_ADDRESS,
        .rate = MMA8X5X_RATE_50HZ,
        .range = MMA8X5X_RANGE_2G
    };
    
    if (mma8x5x_init(&dev, &params) != MMA8X5X_OK) {
        puts("Failed to initialize MMA8x5x");
        return 1;
    }

    while (1) {
        read_sensor();
        xtimer_sleep(5); // Sleep for 5 seconds
    }

    return 0;
}