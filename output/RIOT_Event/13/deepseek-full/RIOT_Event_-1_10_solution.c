
#include "ztimer.h"
#include "thread.h"
#include "msg.h"
#include "mutex.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdlib.h"
#include "stdio.h"
#include "unistd.h"
#include "stdatomic.h"
#include "esp_system.h"

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    ztimer_t timer;

    // Initialize the ztimer with the periodic callback
    ztimer_init(&timer, ZTIMER_USEC, periodic_callback, NULL);

    // Start the timer with the periodic interval
    ztimer_set(&timer, PERIODIC_INTERVAL);

    // Main loop to keep the application running
    while (1) {
        // Yield to allow other threads to run
        thread_yield();
    }

    return 0;
}
