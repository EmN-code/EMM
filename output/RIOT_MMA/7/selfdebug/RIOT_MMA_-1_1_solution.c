#include <thread_arch.h>
#include <periph_conf.h>
#include <inttypes.h>
#include "mma8x5x.h"

#define THREAD_PRIORITY_MAIN    (THREAD_PRIORITY_MAIN - 1)
#define THREAD_STACKSIZE        (THREAD_STACKSIZE_MAIN)
#define SLEEP_DURATION          (5U) // Sleep duration in seconds

static char thread_stack[THREAD_STACKSIZE];

// Function to read data from MMA8x5x
static void *read_mma8x5x(void *arg) {
    mma8x5x_t *dev = (mma8x5x_t *)arg;

    while (1) {
        if (mma8x5x_is_ready(dev)) {
            printf("MMA8x5x device is ready. Reading data...\n");
            // (Assuming the use of a function to read actual sensor data)
            // Example: mma8x5x_read_data(dev, &data);
            // printf("Read data: %d\n", data);
        }
        else {
            printf("MMA8x5x device is not ready.\n");
        }

        thread_sleep(SLEEP_DURATION);
    }

    return NULL;
}

int main(void) {
    mma8x5x_t dev;
    mma8x5x_params_t params;

    // Initialize the MMA8x5x device
    if (mma8x5x_init(&dev, &params) != 0) {
        printf("Failed to initialize the MMA8x5x device.\n");
        return 1;
    }

    // Create a thread to read data from MMA8x5x every 5 seconds
    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN, THREAD_CREATE_STACKTEST, read_mma8x5x, &dev, "mma8x5x_reader");

    // Main thread can perform other tasks
    while (1) {
        thread_sleep(60);
    }

    return 0;
}