#include "xtimer.h"
#include "ztimer.h"
#include "timex.h"
#include "stdio.h"
#include "stdlib.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "unistd.h"
#include "msg.h"
#include "mbox.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "time_units.h"
#include "kernel_defines.h"
#include "arpa/inet.h"
#include "netinet/in.h"

#define INTERVAL (1U * US_PER_SEC)

static evtimer_msg_event_t event;
static evtimer_t evtimer;

void event_periodic_callback(void *arg) {
    puts("Event Triggered");
}

int main(void) {
    evtimer_init_msg(&evtimer);

    /* Set up the event */
    event.msg.type = 0x1234;
    event.msg.content.value = 0;
    event.msg.sender_pid = sched_active_pid;

    /* Set the periodic callback */
    event_periodic.event = event;
    event_periodic.callback = event_periodic_callback;

    /* Add the event to the event timer */
    evtimer_add_msg(&evtimer, &event.msg, 0);

    while (1) {
        /* Run the event timer every second */
        evtimer_run(&evtimer);
        xtimer_sleep(1);
    }

    return 0;
}
