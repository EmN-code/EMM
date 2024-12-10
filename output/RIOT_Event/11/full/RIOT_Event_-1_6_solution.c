#include "ztimer.h"
#include "xtimer.h"
#include "msg.h"
#include "mbox.h"
#include "stdlib.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "netinet/in.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "stdbool.h"
#include "stdio.h"
#include "stddef.h"
#include "stdatomic.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "time_units.h"
#include "arpa/inet.h"
#include "kernel_defines.h"
#include "timex.h"
#include "unistd.h"

#define EVTIMER_INTERVAL (1U)  // 1 second interval

static evtimer_t evtimer;

void periodic_event_callback(void *arg) {
    (void)arg; // Avoid unused parameter warning
    printf("Periodic event triggered!\n");
    fflush(stdout);
}

int main(void) {
    msg_init_queue(mbox, MBOX_SIZE);
    evtimer_init_msg(&evtimer);

    evtimer_event_t event;
    event.callback = periodic_event_callback;
    evtimer_add_msg(&evtimer, &event, sched_active_pid);

    ztimer_t timer;
    timer.callback = periodic_event_callback;
    timer.arg = NULL;
    ztimer_set(ZTIMER_SEC, &timer, EVTIMER_INTERVAL);

    while (true) {
        evtimer_run(&evtimer);
        sleep(1);
    }

    return 0;
}
