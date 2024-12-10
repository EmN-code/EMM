#include <net/sock/util.h>
#include <netinet/in.h>
#include <net/gcoap.h>
#include <net/sock/udp.h>
#include <mma8x5x_regs.h>
#include <mma8x5x_params.h>
#include <mma8x5x.h>
#include <mbox.h>
#include <msg.h>
#include <kernel_defines.h>
#include <event/thread.h>
#include <evtimer_mbox.h>
#include <evtimer_msg.h>
#include <stddef.h>
#include <stdatomic.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <thread.h>
#include <time_units.h>
#include <timex.h>
#include <unistd.h>
#include <xtimer.h>
#include <ztimer.h>

static void _callback(void *arg)
{   
   (void)arg;
   puts("Callback executed");
}

int main(void)
{
    puts("RIOT Example -- Periodic Event Timer");

    evtimer_t evtimer;
    evtimer_event_t event;

    /* Initialize the event timer with the callback function */
    evtimer_init(&evtimer, _callback);
    
    /* Add a periodic event to the event timer to trigger every second */
    evtimer_add_periodic(&evtimer, &event, 1e6);

    thread_yield_higher();

    return 0;
}