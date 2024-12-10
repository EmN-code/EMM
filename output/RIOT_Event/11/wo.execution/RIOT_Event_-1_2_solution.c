#include <ztimer.h>
#include <timex.h>
#include <time_units.h>
#include <xtimer.h>
#include <event/periodic.h>
#include <event/thread.h>
#include <event/callback.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <stdio.h>

#define INTERVAL_SEC (1U)  // interval for periodic callback, in seconds

static void event_periodic(void *arg)
{
    (void)arg;
    printf("Periodic event triggered\n");
}

int main(void)
{
    printf("Starting the event periodic test on ESP32\n");

    /* Initialize ztimer and set periodic callback */
    ztimer_periodic_t periodic_event;
    ztimer_periodic_init(ZTIMER_MSEC, &periodic_event, EVENT_PRIO_NORMAL);
    ztimer_periodic_start(&periodic_event, INTERVAL_SEC * MS_PER_SEC, event_periodic, NULL);

    /* Let the main thread idle */
    ztimer_sleep(ZTIMER_MSEC, INTERVAL_SEC);

    return 0;
}