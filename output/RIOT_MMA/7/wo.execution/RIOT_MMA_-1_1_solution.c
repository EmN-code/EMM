#include "thread.h"
#include "mma8x5x.h"
#include "periph_conf.h"

#define READ_INTERVAL (5U)

static mma8x5x_t dev;
static const mma8x5x_params_t params = {
    .params = {0} // Initialize with actual parameters
};

void *mma8x5x_thread(void *arg) {
    (void)arg; // Unused
    while (1) {
        if (mma8x5x_is_ready(&dev)) {
            // Here, the function to read data would be called, e.g.,
            // float x, y, z;
            // mma8x5x_read_xyz(&dev, &x, &y, &z);
            // printf("X: %f, Y: %f, Z: %f\n", x, y, z);
        }
        xtimer_sleep(READ_INTERVAL);
    }
    return NULL;
}

int main(void) {
    if (mma8x5x_init(&dev, &params) != 0) {
        printf("MMA8x5x initialization failed\n");
        return 1;
    }
    
    // Create a thread to read from MMA8x5x sensor every 5 seconds
    thread_create(mma8x5x_thread, NULL, THREAD_STACKSIZE_DEFAULT, "mma8x5x_thread");

    // Main thread can have other tasks or go to sleep
    while(1) {
        thread_sleep();
    }
    return 0;
}