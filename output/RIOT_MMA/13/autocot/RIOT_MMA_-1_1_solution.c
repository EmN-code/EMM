#include <thread_config.h>
#include <thread_arch.h>
#include <thread.h>
#include <periph_conf.h>
#include <inttypes.h>
#include <stdio.h>
#include "mma8x5x.h"

#define SLEEP_INTERVAL 5

static mma8x5x_t dev;

// Thread function to read from the MMA8x5x sensor
void *mma8x5x_read_thread(void *arg) {
    (void)arg;

    // Initialize the MMA8x5x sensor
    mma8x5x_params_t params;
    params.device_id = 0; // Set appropriate device_id if needed
    if (mma8x5x_init(&dev, &params) != 0) {
        printf("Initialization failed\n");
        return NULL;
    }

    while (1) {
        if (mma8x5x_is_ready(&dev)) {
            printf("Device is ready\n");
            // Read sensor data. This is an abstract operation
            // since specific read API is not provided above.
            // You should replace it with actual sensing code.
        } else {
            printf("Device is not ready\n");
        }

        // Sleep for defined interval
        thread_sleep(SLEEP_INTERVAL);
    }
}

int main(void) {
    // Create a thread for reading sensor data
    kernel_pid_t pid = thread_create(
        malloc(STACKSIZE_DEFAULT),
        STACKSIZE_DEFAULT,
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        mma8x5x_read_thread,
        NULL,
        "mma8x5x_read"
    );

    if (pid > 0) {
        printf("Thread created successfully: %d\n", pid);
    } else {
        printf("Failed to create thread\n");
    }

    // Main thread can do other tasks or wait
    while (1) {
        thread_sleep(1000); // sleep for 1 second
    }

    return 0;
}