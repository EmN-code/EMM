#include "ztimer.h"
#include "xtimer.h"
#include "stddef.h"
#include "stdlib.h"
#include "stdio.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "event/thread.h"
#include "msg.h"
#include "mbox.h"
#include "xtimer.h"
#include "evtimer.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "dht_params.h"
#include "mma8x5x.h"
#include "mma8x5x_params.h"
#include "mma8x5x_regs.h"
#include "time_units.h"
#include "timex.h"
#include "kernel_defines.h"
#include "netinet/in.h"
#include "net/emcute.h"
#include "net/gcoap.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "arpa/inet.h"
#include "unistd.h"

#define THREAD_STACKSIZE_MAIN   (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN    (THREAD_PRIORITY_MAIN - 1)
#define EVENT_PERIOD            (1U * US_PER_SEC)

evtimer_t evtimer;
int period = EVENT_PERIOD;

static void _evtimer_callback(void *arg)
{
    (void)arg;
    printf("Event triggered!\n");
}

int main(void)
{
    printf("RIOT Message Box Example\n");

    /* Create message queue */
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    /* Initialize event timer */
    evtimer_init_msg(&evtimer);

    /* Set up event with callback */
    evtimer_msg_event_t event;
    event.event.arg = NULL;
    event.event.callback = _evtimer_callback;
    event.event.period = period;

    /* Start the timer */
    evtimer_add_msg(&evtimer, &event, KERNEL_PID_MAIN);

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        xtimer_sleep(period);
    }

    return 0;
}