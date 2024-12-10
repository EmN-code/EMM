#include "thread.h"
#include "periph_conf.h"
#include "mma8x5x.h"

#define MMA8X5X_READ_INTERVAL (5U)

static kernel_pid_t mma8x5x_thread_pid;
static char mma8x5x_thread_stack[THREAD_STACKSIZE_DEFAULT];

static void *mma8x5x_thread(void *arg) {
    mma8x5x_t *dev = (mma8x5x_t *)arg;
    
    while(1) {
        if (mma8x5x_is_ready(dev)) {
            printf("Device is ready to read\n");

            // Read data from the sensor
            // Replace with actual reading process if needed
            // Example: int data = mma8x5x_read(dev);
            
            printf("Reading data from MMA8x5x sensor\n");
        } else {
            printf("Device is not ready\n");
        }

        // Sleep for the defined interval
        xtimer_sleep(MMA8X5X_READ_INTERVAL);
    }
    return NULL;
}

int main(void) {
    mma8x5x_t dev;
    mma8x5x_params_t params = {0}; // Set proper initialization params as needed

    if (mma8x5x_init(&dev, &params) != 0) {
        printf("Failed to initialize MMA8x5x\n");
        return 1;
    }

    // Start the thread with default stack size and priority
    mma8x5x_thread_pid = thread_create(mma8x5x_thread_stack, sizeof(mma8x5x_thread_stack), 
                                       THREAD_PRIORITY_MAIN - 1, THREAD_CREATE_STACKTEST, 
                                       mma8x5x_thread, &dev, "mma8x5x_thread");

    if (mma8x5x_thread_pid <= KERNEL_PID_UNDEF) {
        printf("Error initializing thread\n");
        return 1;
    }

    return 0;
}