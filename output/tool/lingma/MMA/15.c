#include <stdio.h>
#include <stdlib.h>
#include "board.h"
#include "xtimer.h"
#include "periph/i2c.h"
#include "mma8x5x.h"

// Define the I2C bus and address for the MMA8x5x sensor
#define I2C_BUS         (I2C_DEV(0))
#define MMA8X5X_ADDRESS (MMA8X5X_ADDR)

// Function to initialize the MMA8x5x sensor
static int init_sensor(void) {
    // Initialize the I2C bus
    i2c_init(I2C_BUS, I2C_SPEED_FAST);

    // Initialize the MMA8x5x sensor
    if (mma8x5x_init(I2C_BUS, MMA8X5X_ADDRESS) != MMA8X5X_OK) {
        printf("Failed to initialize MMA8x5x sensor\n");
        return -1;
    }

    // Set the sensor to active mode
    if (mma8x5x_set_mode(MMA8X5X_MODE_ACTIVE) != MMA8X5X_OK) {
        printf("Failed to set MMA8x5x to active mode\n");
        return -1;
    }

    return 0;
}

// Function to read data from the MMA8x5x sensor
static void read_sensor_data(void) {
    int16_t x, y, z;

    // Read the sensor data
    if (mma8x5x_read_data(&x, &y, &z) != MMA8X5X_OK) {
        printf("Failed to read data from MMA8x5x sensor\n");
        return;
    }

    // Print the sensor data
    printf("X: %d, Y: %d, Z: %d\n", x, y, z);
}

int main(void) {
    // Initialize the board
    board_init();

    // Initialize the sensor
    if (init_sensor() != 0) {
        return 1;
    }

    // Main loop
    while (1) {
        // Read and print sensor data
        read_sensor_data();

        // Wait for 5 seconds
        xtimer_sleep(5);
    }

    return 0;
}