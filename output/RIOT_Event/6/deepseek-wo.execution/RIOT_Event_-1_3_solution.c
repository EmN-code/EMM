
#include <stdlib.h>
#include <stdio.h>
#include <msg.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <unistd.h>
#include <timex.h>
#include <stdbool.h>
#include <stddef.h>
#include <mbox.h>
#include <time_units.h>
#include <xtimer.h>
#include <evtimer_msg.h>

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static evtimer_msg_t evtimer;

void periodic_callback(void *arg)
{
    (void)arg;  // Unused parameter
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    evtimer_init_msg(&evtimer);

    evtimer_msg_event_t event;
    event.event.callback = periodic_callback;
    event.event.arg = NULL;

    while (1) {
        evtimer_add_msg(&evtimer, &event, sched_active_pid());
        ztimer_sleep(ZTIMER_USEC, PERIODIC_INTERVAL);
    }

    return 0;
}
