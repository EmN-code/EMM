#include "ztimer.h"
#include "time_units.h"
#include "xtimer.h"
#include "msg.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"
#include "mbox.h"
#include "stdlib.h"
#include "stdbool.h"
#include "stdatomic.h"
#include "stddef.h"
#include "stdio.h"
#include "timex.h"
#include "unistd.h"
#include "kernel_defines.h"
#include "arpa/inet.h"
#include "mma8x5x_regs.h"
#include "netinet/in.h"
#include "net/sock/util.h"
#include "net/sock/udp.h"
#include "net/emcute.h"
#include "net/gcoap.h"

#define INTERVAL_SEC (1U)
#define THREAD_STACKSIZE_MAIN    (THREAD_STACKSIZE_DEFAULT)
#define THREAD_PRIORITY_MAIN     (THREAD_PRIORITY_MAIN - 1)

static evtimer_msg_event_t periodic_event;
static msg_t msg_queue[8];

void periodic_callback(void *arg)
{
    (void)arg;
    puts("Callback triggered every second.");
}

int main(void)
{
    msg_init_queue(msg_queue, 8);
    evtimer_msg_init(&periodic_event);
    evtimer_msg_set(&periodic_event);
    evtimer_msg_set_every(&periodic_event, 1 * XTIMER_MSEC(1000));
    periodic_event.callback = periodic_callback;

    while (1) {
        msg_t m;
        msg_receive(&m);
    }

    return 0;
}