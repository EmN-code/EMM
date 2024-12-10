
#include <stdio.h>
#include "xtimer.h"
#include "event/callback.h"

#define INTERVAL (1 * MS_PER_SEC)  // 1 second interval

static void periodic_callback(void *arg)
{
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    event_periodic_callback_t periodic_event;

    event_periodic_callback_init(&periodic_event, ZTIMER_MSEC, &periodic_callback, NULL);
    event_periodic_callback_set_interval(&periodic_event, INTERVAL);
    event_periodic_callback_start(&periodic_event);

    while (1) {
        xtimer_sleep(1);  // Sleep to prevent busy-waiting
    }

    return 0;
}
