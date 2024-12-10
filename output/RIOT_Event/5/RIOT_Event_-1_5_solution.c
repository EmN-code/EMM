
#include "xtimer.h"
#include "thread.h"
#include "stdio.h"
#include "stddef.h"
#include "stdbool.h"
#include "stdlib.h"
#include "unistd.h"
#include "time_units.h"
#include "stdatomic.h"
#include "kernel_defines.h"

#define INTERVAL_MS (1000U)  // 1 second interval

static void periodic_callback(void *arg)
{
    (void)arg;  // Unused parameter
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    xtimer_t timer;

    // Initialize the timer
    xtimer_init(&timer);

    // Set the periodic callback
    xtimer_set_periodic(&timer, INTERVAL_MS, periodic_callback, NULL);

    // Main loop to keep the application running
    while (1) {
        // Yield to other threads
        thread_yield();
    }

    return 0;
}
