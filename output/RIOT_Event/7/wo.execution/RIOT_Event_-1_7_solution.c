#include "netinet/in.h"
#include "mbox.h"
#include "kernel_defines.h"
#include "arpa/inet.h"

#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "xtimer.h"
#include "ztimer.h"

#include "timex.h"
#include "unistd.h"
#include "time_units.h"
#include "stddef.h"
#include "stdatomic.h"
#include "stdbool.h"
#include "stdio.h"
#include "stdlib.h"
#include "msg.h"

#define INTERVAL_USEC 1000000

static evtimer_t evtimer;
static evtimer_msg_event_t event;

void my_event_handler(void *arg) {
    printf("Event triggered!\n");
}

int main(void) {
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);
    
    evtimer_msg_handler_init(&evtimer);
    
    event.handler = my_event_handler;
    event.event.periodic.interval = INTERVAL_USEC;
    evtimer_msg_add_timer(&evtimer, &event.event, THREAD_GETPID());

    while (1) {
        msg_t msg;
        msg_receive(&msg);
        evtimer_msg_handle_event(&evtimer, &msg);
    }
    
    return 0;
}