
#include <stdlib.h>
#include <stdio.h>
#include <msg.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <unistd.h>
#include <kernel_defines.h>
#include <timex.h>
#include <stdbool.h>
#include <stddef.h>
#include <mbox.h>
#include <time_units.h>
#include <xtimer.h>
#include <evtimer_msg.h>

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static evtimer_t periodic_timer;

static void periodic_callback(void *arg)
{
    (void)arg;  // Unused parameter
    printf("Periodic event triggered!\n");
}

int main(void)
{
    // Initialize the event timer
    evtimer_init(&periodic_timer, periodic_callback);

    // Create an event for the periodic timer
    evtimer_event_t periodic_event;

    // Add the event to the timer with the specified interval
    evtimer_add(&periodic_timer, &periodic_event, PERIODIC_INTERVAL);

    // Keep the thread alive
    while (1) {
        xtimer_sleep(1);  // Sleep for 1 second to keep the thread alive
    }

    return 0;
}
