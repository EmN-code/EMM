#include "ztimer.h"
#include "xtimer.h"
#include "time_units.h"
#include "timex.h"
#include "event/thread.h"
#include "event/callback.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include <stdio.h>

#define INTERVAL_SEC (1U)
#define INTERVAL_USEC (INTERVAL_SEC * US_PER_SEC)

static msg_t msg_queue[8];
static evtimer_t evtimer;
static evtimer_msg_event_t evtimer_event;

void event_handler(evtimer_event_t *event) {
    (void)event;
    printf("Event triggered every second.\n");
}

int main(void) {
    msg_init_queue(msg_queue, 8);
    evtimer_init_msg(&evtimer);
    evtimer_event.event.period = INTERVAL_USEC;
    evtimer_event.event.callback = event_handler;
    evtimer_add_mbox(&evtimer, &evtimer_event, &msg_queue[0].content.ptr);
    xtimer_sleep(INTERVAL_SEC);

    return 0;
}