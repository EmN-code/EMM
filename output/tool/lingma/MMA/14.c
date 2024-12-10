#include <stdio.h>
#include <stdlib.h>
#include "xtimer.h"
#include "periph/i2c.h"
#include "mma8x5x.h"

// I2C configuration
#define I2C_DEV     I2C_DEV(0)  // Use I2C device 0
#define I2C_SPEED   I2C_SPEED_FAST  // Set I2C speed to fast mode

// MMA8x5x configuration
#define MMA8X5X_ADDR 0x1D  // Default I2C address for MMA8x5x

// Function to initialize the I2C bus
void i2c_init(void) {
    i2c_init_dev(I2C_DEV);
    i2c_acquire(I2C_DEV);
    i2c_set_speed(I2C_DEV, I2C_SPEED);
}

// Function to initialize the MMA8x5x sensor
void mma8x5x_init(void) {
    if (mma8x5x_init(I2C_DEV, MMA8X5X_ADDR) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x sensor\n");
        return;
    }
    printf("MMA8x5x sensor initialized successfully\n");
}

// Function to read data from the MMA8x5x sensor
void read_mma8x5x_data(void) {
    mma8x5x_data_t data;

    if (mma8x5x_read_data(I2C_DEV, MMA8X5X_ADDR, &data) != MMA8X5X_OK) {
        printf("Failed to read data from MMA8x5x sensor\n");
        return;
    }

    printf("X: %d, Y: %d, Z: %d\n", data.x, data.y, data.z);
}

// Main function
int main(void) {
    // Initialize the I2C bus
    i2c_init();

    // Initialize the MMA8x5x sensor
    mma8x5x_init();

    // Main loop to read data every 5 seconds
    while (1) {
        read_mma8x5x_data();
        xtimer_sleep(5);  // Sleep for 5 seconds
    }

    return 0;
}