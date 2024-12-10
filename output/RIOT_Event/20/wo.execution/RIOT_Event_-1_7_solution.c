#include "stdlib.h"
#include "stdbool.h"
#include "stdio.h"
#include "netinet/in.h"
#include "msg.h"
#include "mutex.h"

#include "time_units.h"
#include "thread.h"  // Corrected this include order
#include "ztimer.h" 
#include "evtimer_msg.h"

#define INTERVAL (1U * US_PER_SEC)

static evtimer_msg_event_t periodic_event;
static kernel_pid_t target_pid; // Assuming it's defined elsewhere in the code

void callback(void *arg) {
    printf("Event triggered every second!\n");
}

int main(void) {
    evtimer_t evtimer;
    evtimer_init_msg(&evtimer);

    target_pid = thread_getpid();

    periodic_event.event.callback = callback;
    periodic_event.event.next = NULL;

    evtimer_add_msg(&evtimer, &periodic_event, target_pid);

    printf("Event timer initialized.\n");

    while (1) {
        evtimer_run(&evtimer);
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }
    return 0;
}