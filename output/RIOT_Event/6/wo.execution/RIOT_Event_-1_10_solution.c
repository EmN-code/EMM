#include "ztimer.h"
#include "mutex.h"
#include "mbox.h"
#include "msg.h"
#include "stdlib.h"
#include "stdio.h"
#include "evtimer_mbox.h"
#include "unistd.h"
#include "stddef.h"

#define TIMER_INTERVAL (1U * US_PER_SEC)

static evtimer_t my_evtimer;
mbox_t my_mbox;
msg_t mbox_msg_queue[8];  // msg_t array of size power of two

void task_callback(void *arg)
{
    (void)arg;
    puts("Task executed.");
}

int main(void)
{
    evtimer_mbox_event_t my_event;

    // Initialize mbox
    mbox_init(&my_mbox, mbox_msg_queue, 8);
    
    // Initialize the event timer for mbox events
    evtimer_init_mbox(&my_evtimer);

    // Set up event
    my_event.event.period = TIMER_INTERVAL;
    my_event.event.callback = task_callback;
    my_event.mbox = &my_mbox;

    // Add the event to the event timer
    evtimer_add_mbox(&my_evtimer, &my_event, &my_mbox);

    while (1) {
        // Let the timer wait and process
        evtimer_mbox_event_t evt;
        mbox_get(&my_mbox, &evt.event, 0);
        evtimer_handle_one(&my_evtimer);
    }

    return 0;
}