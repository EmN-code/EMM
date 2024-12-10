
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static event_t periodic_event;
static xtimer_t periodic_timer;

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered\n");
}

int main(void)
{
    // Initialize the periodic event
    event_periodic_init(&periodic_event, &periodic_callback, NULL);

    // Initialize the periodic timer
    periodic_timer.callback = (xtimer_callback_t)event_post;
    periodic_timer.arg = &periodic_event;

    // Set the timer to trigger the event every second
    xtimer_periodic_wakeup(&periodic_timer.target, PERIODIC_INTERVAL);

    // Wait for events
    while (1) {
        event_wait(&periodic_event);
    }

    return 0;
}
