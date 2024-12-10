#include "xtimer.h"
#include "ztimer.h"
#include "unistd.h"
#include "timex.h"
#include "time_units.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "stdio.h"

void callback(void *arg) {
    printf("Callback executed\n");
}

int main(void) {
    evtimer_msg_t evtimer;
    evtimer_event_t event;
    ztimer_init(ZTIMER_USEC);

    // Setup the event
    evtimer_msg_event_t evt_msg;
    evt_msg.event.callback = callback;
    evt_msg.event.period = 1000000; // 1 second in microseconds
    evtimer_add_msg(&evtimer, &evt_msg, thread_getpid());

    puts("Event timer initialized and running");

    while (1) {
        ztimer_sleep(ZTIMER_USEC, 1000000);
    }

    return 0;
}