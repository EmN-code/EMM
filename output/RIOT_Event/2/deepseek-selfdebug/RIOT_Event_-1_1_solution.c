
#include <stdio.h>
#include "event/periodic_callback.h"
#include "ztimer.h"
#include "thread.h"

#define INTERVAL_SEC (1 * 1000000U)  // 1 second in microseconds

static event_periodic_callback_t periodic_event;

void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    // Initialize the periodic event
    event_periodic_callback_init(&periodic_event, ZTIMER_USEC, NULL, periodic_callback);

    // Start the periodic event with an interval of 1 second
    event_periodic_callback_start(&periodic_event, INTERVAL_SEC);

    // Keep the main thread alive
    while (1) {
        thread_yield();
    }

    return 0;
}
