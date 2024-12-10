
#include <stdlib.h>
#include <msg.h>
#include <mma8x5x_regs.h>
#include <mma8x5x.h>
#include <time_units.h>
#include <xtimer.h>
#include <mma8x5x_params.h>
#include <stdio.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <kernel_defines.h>
#include <evtimer_mbox.h>
#include <netinet/in.h>
#include <timex.h>
#include <evtimer_msg.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stddef.h>
#include <mbox.h>

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    evtimer_t evtimer;
    evtimer_init(&evtimer, NULL);

    evtimer_event_t event;
    event.event.handler = periodic_callback;
    event.event.arg = NULL;

    while (1) {
        evtimer_add_event(&evtimer, &event, PERIODIC_INTERVAL);
        xtimer_sleep(PERIODIC_INTERVAL);
    }

    return 0;
}
