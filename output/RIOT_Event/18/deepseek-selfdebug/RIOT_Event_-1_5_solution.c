
#include "xtimer.h"
#include "event/periodic.h"
#include "event/callback.h"

#define INTERVAL_MS (1000U)  // 1 second interval

static event_periodic_t periodic_event;
static event_t event;

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    // Initialize the periodic event
    event_periodic_init(&periodic_event, xtimer_ticks_from_usec(INTERVAL_MS * 1000), periodic_callback, NULL);

    // Initialize the event
    event_init(&event, periodic_callback, NULL);

    // Add the event to the periodic event
    event_periodic_add_event(&periodic_event, &event);

    // Start the periodic event
    event_periodic_start(&periodic_event);

    // Main loop to keep the application running
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
