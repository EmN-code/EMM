
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static event_t periodic_event;
static xtimer_t periodic_timer;

static void periodic_callback(void *arg)
{
    (void)arg;
    event_post(EVENT_PRIO_MEDIUM, &periodic_event);
}

int main(void)
{
    // Initialize the periodic event
    event_periodic_init(&periodic_timer, PERIODIC_INTERVAL, periodic_callback, NULL);

    // Start the periodic timer
    xtimer_periodic_wakeup(&periodic_timer.timer, PERIODIC_INTERVAL);

    while (1) {
        // Wait for the periodic event
        event_wait(&periodic_event);
        puts("Periodic event triggered!");
    }

    return 0;
}
