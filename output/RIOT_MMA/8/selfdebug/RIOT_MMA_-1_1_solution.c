#include "thread.h"
#include "periph_conf.h"
#include "mma8x5x.h"

#define STACK_SIZE_THREAD (1024)
#define THREAD_PRIORITY   (THREAD_PRIORITY_MAIN - 1)
#define MMA_TASK_STACKSIZE THREAD_STACKSIZE_DEFAULT

static char thread_stack[STACK_SIZE_THREAD];

static void *mma_read_thread(void *arg) {
    mma8x5x_t *dev = (mma8x5x_t *)arg;
    while (1) {
        if (mma8x5x_is_ready(dev)) {
            printf("MMA8x5x is ready to be read\n");
            // Read from the MMA8x5x sensor here
            // Implement reading logic
        } else {
            printf("MMA8x5x is not ready\n");
        }
        /* Sleep for 5 seconds */
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    mma8x5x_params_t params;
    mma8x5x_t dev;

    // Initialize sensor parameters here
    // params.bus = I2C_BUS_NUMBER; Add other initializations

    if (mma8x5x_init(&dev, &params) == 0) {
        printf("MMA8x5x initialized\n");
    } else {
        printf("Failed to initialize MMA8x5x\n");
        return 1;
    }

    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY, THREAD_CREATE_STACKTEST, mma_read_thread, &dev, "mma_read");
    return 0;
}