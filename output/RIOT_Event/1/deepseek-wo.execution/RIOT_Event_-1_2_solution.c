
#include "time_units.h"
#include "ztimer.h"
#include "thread.h"
#include "xtimer.h"
#include "event/thread.h"
#include "timex.h"
#include "event/periodic_callback.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

#define INTERVAL_SEC (1U)

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    event_periodic_callback_t event;
    event_queue_t queue;

    event_queue_init(&queue);

    /* Initialize the periodic callback event */
    event_periodic_callback_init(&event, ZTIMER_MSEC, &queue, periodic_callback, NULL);

    /* Start the periodic callback event with an interval of 1 second */
    event_periodic_callback_start(&event, INTERVAL_SEC * MS_PER_SEC);

    /* Yield to allow the event to be processed */
    thread_yield();

    return 0;
}
