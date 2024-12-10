#include "thread_arch.h"
#include "thread.h"
#include "periph_conf.h"
#include "mma8x5x.h"
#include <stdio.h>

// Function prototype
void *read_mma8x5x(void *arg);

// Main function
int main(void)
{
    // Initialize MMA8x5x sensor
    mma8x5x_t sensor;
    mma8x5x_params_t params;
    
    if (mma8x5x_init(&sensor, &params) != 0) {
        printf("Failed to initialize the MMA8x5x sensor\n");
        return 1;
    }

    // Create a new thread for reading sensor data
    char stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(stack, sizeof(stack), THREAD_PRIORITY_MAIN - 1,
                  0, read_mma8x5x, &sensor, "mma8x5x_thread");
    
    return 0;
}

// Thread function to read data from MMA8x5x sensor
void *read_mma8x5x(void *arg)
{
    mma8x5x_t *sensor = (mma8x5x_t *)arg;
    
    while (1) {
        if (mma8x5x_is_ready(sensor)) {
            // Assuming hypothetical function to read sensor data
            int x, y, z;
            mma8x5x_read(sensor, &x, &y, &z);
            printf("Readings - x: %d, y: %d, z: %d\n", x, y, z);
        }
        // Sleep for 5 seconds between readings
        thread_sleep_s(5);
    }
    return NULL;
}