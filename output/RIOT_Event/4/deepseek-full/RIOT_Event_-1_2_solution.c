
#include "xtimer.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "time_units.h"
#include "event/thread.h"
#include "ztimer.h"
#include "timex.h"
#include "thread.h"
#include "event/periodic_callback.h"

#define INTERVAL_SEC (1U)

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    event_periodic_callback_t event;
    ztimer_clock_t *clock = ZTIMER_USEC;
    event_queue_t queue;

    event_queue_init(&queue);

    event_periodic_callback_init(&event, clock, &queue, periodic_callback, NULL);
    event_periodic_callback_start(&event, INTERVAL_SEC * US_PER_SEC);

    while (1) {
        event_process(&queue);
    }

    return 0;
}
