
#include <stdio.h>
#include "thread.h"
#include "xtimer.h"
#include "event/periodic.h"

#define PERIODIC_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    puts("Starting periodic event example");

    // Initialize the periodic event timer
    event_periodic_t periodic_event;
    event_periodic_init(&periodic_event, xtimer_now_usec64(), PERIODIC_INTERVAL, periodic_callback, NULL);

    // Start the periodic event timer
    event_periodic_start(&periodic_event);

    // Keep the main thread alive
    while (1) {
        thread_yield();
    }

    return 0;
}
