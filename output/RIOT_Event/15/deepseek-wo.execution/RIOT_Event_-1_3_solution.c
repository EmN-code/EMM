
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

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    evtimer_t evtimer;
    evtimer_init_msg(&evtimer);

    msg_t msg;
    msg.content.ptr = NULL;
    msg.type = 0;

    while (1) {
        evtimer_add_msg(&evtimer, &msg, ztimer_now(ZTIMER_USEC) + PERIODIC_INTERVAL);
        msg_receive(&msg);
        periodic_callback(NULL);
    }

    return 0;
}
