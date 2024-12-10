#include "xtimer.h"
#include "ztimer.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "msg.h"
#include "stdio.h"

static evtimer_t evtimer;
static evtimer_msg_event_t event;
msg_t msg_queue[8];

void timer_callback(void *arg) {
    (void)arg;
    printf("Timer expired\n");
    // Add LED toggle functionality here
}

int main(void)
{
    // Initialize message queue
    msg_init_queue(msg_queue, 8);

    // initialize the event timer
    evtimer_init_msg(&evtimer);
    event.msg.content.ptr = NULL;
    event.event.callback = timer_callback;

    // Set timer to trigger every second (1,000,000 microseconds)
    ztimer_periodic_wakeup(ztimer_now(), 1000000);

    while (1) {
        msg_receive(&event.msg);
        evtimer_add_msg(&evtimer, &event, thread_getpid());
    }

    return 0;
}