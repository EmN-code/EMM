#include <thread.h>
#include <periph_conf.h>
#include <inttypes.h>
#include <stdio.h>

#include "mma8x5x.h"  // Include the header for the MMA8x5x sensor

// Setup the params for the MMA8x5x
static const mma8x5x_params_t mma_params = {
    .params = { /* device-specific init parameters */ }
};

// This is the device instance that we'll use for interacting with the MMA8x5x
static mma8x5x_t dev;

// Function to initialize the sensor
void init_sensor(void) {
    if (mma8x5x_init(&dev, &mma_params) != 0) {
        printf("Initialization of MMA8x5x failed!\n");
        return;
    }
    printf("MMA8x5x initialized successfully.\n");
}

// Function to read sensor data
void read_sensor_data(void) {
    if (mma8x5x_is_ready(&dev)) {
        // Assuming x, y, z are the coordinate values
        int16_t x, y, z;
        x = y = z = 0;  // Initialize to zero
        // Function to read x, y, z axis, presuming such a function exists
        // Replace with actual function calls as necessary
        // mma8x5x_read_xyz(&dev, &x, &y, &z);

        printf("X: %d, Y: %d, Z: %d\n", x, y, z);
    } else {
        printf("Sensor is not ready.\n");
    }
}

// The main loop thread
static char stack[THREAD_STACKSIZE_MAIN];
void *sensor_loop(void *arg) {
    (void)arg;

    init_sensor();

    while (1) {
        read_sensor_data();
        // Sleep for 5 seconds
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    puts("Starting the sensor reading application...");

    // Create a thread for our main loop
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0, sensor_loop, NULL, "sensor_loop");

    // Do nothing in main
    while (1) {
        thread_yield();
    }

    return 0;
}