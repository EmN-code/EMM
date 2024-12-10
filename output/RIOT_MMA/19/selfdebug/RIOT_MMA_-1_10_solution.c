#include <xtimer.h>
#include <thread.h>
#include <kernel.h>
#include <periph/i2c.h>
#include <stdio.h>

#define THREAD_PRIORITY (THREAD_PRIORITY_MAIN - 1)
#define THREAD_STACK_SIZE (THREAD_STACKSIZE_MAIN + THREAD_CREATE_STACKTEST)

static char stack[THREAD_STACK_SIZE];

static void *thread_handler(void *arg) {
    (void)arg;
    while (1) {
        // Placeholder for MMA8x5x read function
        printf("Reading MMA8x5x accelerometer data...\n");
        
        xtimer_sleep(5);
    }
    return NULL;
}

int main(void) {
    // Placeholder for MMA8x5x initialization

    // Create a thread to read MMA8x5x data every 5 seconds
    thread_create(stack, sizeof(stack), THREAD_PRIORITY, THREAD_CREATE_STACKTEST, 
                  thread_handler, NULL, "mma8x5x_reader_thread");

    return 0;
}