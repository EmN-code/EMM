
#include "xtimer.h"
#include "event/callback.h"
#include "event/timeout.h"
#include "event/periodic.h"

#define INTERVAL_MS (1000U)  // 1 second interval

static void periodic_callback(void *arg)
{
    (void)arg;  // Unused argument
    printf("Periodic event triggered!\n");
}

int main(void)
{
    event_periodic_t periodic_event;
    event_periodic_init(&periodic_event, xtimer_ticks32(INTERVAL_MS), periodic_callback, NULL);
    event_periodic_add_event(&periodic_event);
    event_periodic_start(&periodic_event);

    while (1) {
        event_wait(&periodic_event.event);
    }

    return 0;
}
