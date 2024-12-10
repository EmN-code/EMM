#include "thread.h"
#include "periph_conf.h"
#include "mma8x5x.h"

#include <stdio.h>
#include <xtimer.h>

// Define global MMA8x5x device structure and parameters
mma8x5x_t dev;
mma8x5x_params_t params = {
    // Parameters need to be set according to the hardware specifics
};

// Initialization function for MMA8x5x
int mma8x5x_initialize(void) {
    // Initialize the MMA8x5x sensor
    if (mma8x5x_init(&dev, &params) != 0) {
        printf("Failed to initialize MMA8x5x sensor\n");
        return -1;
    }
    return 0;
}

// Main function to collect and print sensor data
void *mma8x5x_reader_thread(void *arg) {
    (void)arg;
    while (1) {
        int16_t x, y, z;
        if (mma8x5x_read_accel(&dev, &x, &y, &z) == 0) {
            printf("Acceleration [G]: X=%.2f, Y=%.2f, Z=%.2f\n", (float)x/1000, (float)y/1000, (float)z/1000);
        } else {
            printf("Failed to read acceleration data\n");
        }
        xtimer_sleep(5); // Sleep for 5 seconds before next read
    }
    return NULL;
}

int main(void) {
    puts("MMA8x5x sensor reader starting up...");

    // Initialize the MMA8x5x sensor
    if (mma8x5x_initialize() != 0) {
        return 1; // Exit if initialization fails
    }

    // Create a thread to read the sensor data
    char stack[THREAD_STACKSIZE_MAIN];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1, 0,
                  mma8x5x_reader_thread, NULL, "mma8x5x_reader");

    // Let the main thread sleep forever
    thread_sleep();
    return 0;
}