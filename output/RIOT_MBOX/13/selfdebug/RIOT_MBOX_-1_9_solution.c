#include "xtimer.h"
#include "sys/types.h"
#include "msg.h"
#include "kernel_types.h"
#include "evtimer.h"
#include "cib.h"
#include "stdio.h"

#define QUEUE_SIZE 8
#define INTERVAL_US (1U * US_PER_SEC)

int main(void) {
    static msg_t msg_queue[QUEUE_SIZE];
    evtimer_t evtimer;
    evtimer_event_t event;
    mbox_t mbox;

    // Initialize message box
    mbox_init(&mbox, msg_queue, QUEUE_SIZE);

    // Initialize event timer
    evtimer_init(&evtimer, &event);

    // Initialize the event
    event.callback = (evtimer_callback_t) puts;
    event.arg = "Event triggered!\n";

    // Add the event to the timer
    evtimer_add(&evtimer, &event, INTERVAL_US);

    printf("Event timer initialized, sending events every %u seconds.\n", INTERVAL_US / US_PER_SEC);

    while(1) {
        msg_t msg;
        msg_receive(&msg);
    }

    return 0;
}