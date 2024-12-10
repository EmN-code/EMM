#include <thread.h>
#include <periph_conf.h>
#include "mma8x5x.h"

// Define the thread stack size and priority
#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_MAIN)

// Define MMA8x5x parameters
mma8x5x_t sensor;
mma8x5x_params_t sensor_params = {
    .params = {0}, // assuming default parameters are acceptable
};

// This function reads data from the MMA8x5x sensor
void *read_sensor(void *arg) {
    (void)arg;
    // Initialize sensor
    if (mma8x5x_init(&sensor, &sensor_params) != 0) {
        // Handle initialization error
        printf("Failed to initialize MMA8x5x\n");
        return NULL;
    }

    while (1) {
        int16_t x, y, z;
        
        // Read sensor
        if (mma8x5x_read(&sensor, &x, &y, &z) == 0) {
            printf("Acceleration data: X=%d, Y=%d, Z=%d\n", x, y, z);
        }
        else {
            printf("Failed to read data from MMA8x5x\n");
        }

        // Sleep for 5 seconds
        xtimer_sleep(5);
    }

    return NULL;
}

int main(void) {
    // Create a thread for the sensor readings
    thread_create(
        stack,
        sizeof(stack),
        THREAD_PRIORITY_MAIN,
        THREAD_CREATE_STACKTEST,
        read_sensor,
        NULL,
        "read_sensor_thread"
    );

    return 0;
}