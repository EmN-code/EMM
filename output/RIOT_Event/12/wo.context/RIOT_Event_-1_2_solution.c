#include "xtimer.h"
#include "event/periodic.h"
#include "event/callback.h"
#include <stdio.h>

#define INTERVAL (1U * US_PER_SEC)  // 1-second interval

static event_periodic_t periodic_event;

static void timer_callback(void *arg)
{
    (void)arg;
    // Add your periodic task here
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    // Initialize the periodic event
    event_periodic_init(&periodic_event, INTERVAL, timer_callback, NULL);

    // Start the periodic event
    event_periodic_start(&periodic_event);

    printf("Periodic event started, triggering every second.\n");

    // Let the main loop run indefinitely
    while (1) {
        // Sleep for a short period to reduce CPU usage
        xtimer_sleep(1);
    }

    return 0;
}