#include "thread.h"
#include "periph_conf.h"
#include "thread_flags.h"
#include "host/ble_hs_adv.h"
#include "inttypes.h"
#include "mma8x5x.h"

void* mma8x5x_read_thread(void *arg) {
    mma8x5x_t *dev = (mma8x5x_t*)arg;
    while (1) {
        if (mma8x5x_is_ready(dev)) {
            // Assuming a structure of axes exists
            // Replace with actual read function and processing
            printf("Device is ready. Reading values...\n");
            // (e.g., mma8x5x_read_axis(dev, axis_data))
        }
        else {
            printf("Device not ready.\n");
        }
        // Sleep for 5 seconds before the next read
        thread_sleep_s(5);
    }
    return NULL;
}

int main(void) {
    // Declare a device and parameter structure
    mma8x5x_t dev;
    mma8x5x_params_t params;
    
    // Initialize the device
    if (mma8x5x_init(&dev, &params) != 0) {
        printf("Failed to initialize MMA8x5x\n");
        return 1;
    }
    
    // Create thread for periodic sensor read
    thread_create(NULL, THREAD_STACKSIZE_DEFAULT, THREAD_PRIORITY_MAIN,
                  THREAD_CREATE_STACKTEST, mma8x5x_read_thread, &dev,
                  "mma8x5x_read");

    // Keep the main thread active
    while (1) {
        thread_sleep();
    }

    return 0;
}