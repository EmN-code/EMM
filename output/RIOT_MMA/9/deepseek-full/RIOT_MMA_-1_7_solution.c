
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <thread.h>
#include <ztimer.h>

#define STACKSIZE 1024
#define PRIORITY 5
#define SLEEP_INTERVAL (5 * ZTIMER_SEC)

static char thread_stack[STACKSIZE];

void *accelerometer_thread(void *arg)
{
    (void)arg;

    while (true) {
        // Simulate reading accelerometer data
        int16_t x = rand() % 1000; // Simulated X-axis data
        int16_t y = rand() % 1000; // Simulated Y-axis data
        int16_t z = rand() % 1000; // Simulated Z-axis data

        printf("Accelerometer Data: X=%d, Y=%d, Z=%d\n", x, y, z);

        // Sleep for 5 seconds
        ztimer_sleep(ZTIMER_SEC, SLEEP_INTERVAL);
    }

    return NULL;
}

int main(void)
{
    // Create the accelerometer reading thread
    thread_create(thread_stack, STACKSIZE, PRIORITY, 0, accelerometer_thread, NULL, "accelerometer");

    // Main thread can continue with other tasks or simply sleep
    while (true) {
        ztimer_sleep(ZTIMER_SEC, 1);
    }

    return 0;
}
