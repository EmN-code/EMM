#include "xtimer.h"
#include <stdio.h>
#include <evtimer_msg.h>
#include <stdlib.h>
#include <kernel_defines.h>

static evtimer_t evtimer;
static evtimer_msg_event_t event;

void callback(void *arg) {
    printf("1 second elapsed\n");
}

int main(void) {
    evtimer_msg_wait_handler_t handler;
    evtimer_init_msg(&evtimer, handler);

    event.event.callback = callback;
    event.event.interval = 1 * US_PER_SEC; // 1 second interval

    evtimer_add_msg(&evtimer, &event, KERNEL_PID_UNDEF);

    while (1) {
        evtimer_periodic_interval(&evtimer, &event.event, 1);
        xtimer_sleep(1);
    }

    return 0;
}