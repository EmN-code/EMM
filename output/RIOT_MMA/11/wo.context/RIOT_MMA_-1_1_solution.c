#include "thread.h"
#include "mma8x5x.h"
#include "xtimer.h"
#include <stdio.h>

#define INTERVAL (5U * US_PER_SEC)  // 5 seconds interval

static mma8x5x_t dev;

void *mma8x5x_read_loop(void *arg) {
    (void) arg;
    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        // Wait for the next wakeup
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL);

        // Read data from MMA8x5x sensor
        int16_t x, y, z;
        if (mma8x5x_read(&dev, &x, &y, &z) == 0) {
            printf("Acceleration Data - X: %d, Y: %d, Z: %d\n", x, y, z);
        } else {
            printf("Error reading from MMA8x5x sensor\n");
        }
    }

    return NULL;
}

int main(void) {
    // Initialize the MMA8x5x sensor
    mma8x5x_params_t params = {
        // Assuming default configuration for the sensor
    };
    if (mma8x5x_init(&dev, &params) != 0) {
        printf("Failed to initialize MMA8x5x sensor\n");
        return 1;
    }

    printf("MMA8x5x sensor initialized successfully\n");

    // Create a thread to read sensor data every 5 seconds
    static char thread_stack[THREAD_STACKSIZE_DEFAULT];
    thread_create(thread_stack, sizeof(thread_stack), THREAD_PRIORITY_MAIN - 1,
                  THREAD_CREATE_STACKTEST, mma8x5x_read_loop, NULL, "mma8x5x_thread");

    // Main thread can perform other tasks or sleep
    while (1) {
        // For now, just sleep indefinitely
        thread_sleep();
    }

    return 0;
}