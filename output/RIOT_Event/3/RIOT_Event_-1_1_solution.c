
#include <stdio.h>
#include "event/periodic_callback.h"
#include "ztimer.h"
#include "thread.h"

#define INTERVAL_MS (1000U)  // 1 second interval

static event_periodic_callback_t periodic_event;

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    // Initialize the periodic event
    event_periodic_callback_init(&periodic_event, ZTIMER_MSEC, NULL, periodic_callback);

    // Start the periodic event with a 1 second interval
    event_periodic_callback_start(&periodic_event, INTERVAL_MS);

    // Main loop to keep the application running
    while (1) {
        // Yield to allow other threads to run
        thread_yield();
    }

    return 0;
}
