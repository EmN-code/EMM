# Necessary imports that the user needs
#include <net/gcoap.h>
#include <net/sock/util.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <kernel_defines.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <timex.h>
#include <msg.h>
#include <mbox.h>
#include <xtimer.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <stdio.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include <unistd.h>
#include <time_units.h>

static void periodic_callback(void *arg)
{
    (void) arg; // avoid unused parameter warning
    printf("Periodic event triggered\n");
}

int main(void)
{
    evtimer_t evtimer;
    evtimer_event_t event;
    evtimer_init(&evtimer, _evtimer_mbox_handler);
    
    event.callback = periodic_callback;
    event.period = 1 * US_PER_SEC;
    
    evtimer_add(&evtimer, &event);

    while (1) {
        msg_t msg;
        mbox_receive(&msg);
        evtimer_mbox_trigger(&msg);
    }
    return 0;
}