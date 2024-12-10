#include <thread.h>
#include <periph_conf.h>
#include <host/ble_ibeacon.h>
#include <hashes/sha2xx_common.h>
#include <inttypes.h>

#define LOOP_INTERVAL 5

// MMA8x5x parameters (example values)
mma8x5x_params_t mma_params = {
    // Initialize with specific parameters
};

// MMA8x5x device
mma8x5x_t mma_device;

void *read_mma8x5x(void *arg) {
    // Initialize MMA8x5x sensor
    if (mma8x5x_init(&mma_device, &mma_params) != 0) {
        printf("MMA8x5x initialization failed!\n");
        return NULL;
    }

    while (1) {
        // Check if MMA8x5x is ready
        if (mma8x5x_is_ready(&mma_device)) {
            // Perform reading here (For example)
            printf("MMA8x5x is ready, perform reading...\n");
        } else {
            printf("MMA8x5x not ready.\n");
        }
        
        // Sleep for LOOP_INTERVAL seconds
        xtimer_sleep(LOOP_INTERVAL);
    }

    return NULL;
}

int main(void) {
    // Initialize necessary peripherals, e.g., timers, UART, etc.

    // Create a thread for reading MMA8x5x sensor
    thread_create(NULL, STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, read_mma8x5x, NULL, "mma_thread");

    return 0;
}