
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
static msg_t msg_queue[8];

void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    msg_init_queue(msg_queue, sizeof(msg_queue));

    evtimer_init_msg(&evtimer);

    evtimer_msg_event_t event;
    event.event.handler = (evtimer_callback_t)periodic_callback;
    event.event.arg = NULL;

    while (1) {
        evtimer_add_msg(&evtimer, &event, sched_active_pid);
        xtimer_sleep(PERIODIC_INTERVAL / US_PER_SEC);
    }

    return 0;
}
