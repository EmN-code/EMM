#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stddef.h"
#include "stdio.h"
#include "stdatomic.h"
#include "msg.h"
#include "mbox.h"
#include "mma8x5x_regs.h"
#include "arpa/inet.h"
#include "netinet/in.h"
#include "unistd.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "kernel_defines.h"
#include "time_units.h"

#define INTERVAL_USEC (1U * US_PER_SEC)

static void event_callback(void *arg) {
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void) {
    evtimer_t evtimer;
    evtimer_event_t event;

    /* Initialize the evtimer */
    evtimer_base_init(&evtimer);

    /* Set the event callback */
    event.event.callback = event_callback;
    event.event.arg = NULL;

    /* Set the time period for the event */
    event.event.time = ztimer_now(ZTIMER_SEC) + INTERVAL_USEC;

    /* Adding an event to the evtimer */
    evtimer_add(&evtimer, &event);

    while (1) {
        /* Sleep to prevent busy-waiting */
        ztimer_sleep(ZTIMER_SEC, 1);
    }
    return 0;
}