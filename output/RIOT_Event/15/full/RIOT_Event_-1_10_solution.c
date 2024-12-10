#include "xtimer.h"
#include "stdlib.h"
#include "stdio.h"
#include "msg.h"
#include "evtimer_mbox.h"

#define SLEEP_INTERVAL (1U * 1000000U) // 1 second in microseconds

static void event_callback(void *arg)
{
    (void)arg;
    printf("Event triggered every second!\n");
}

int main(void)
{
    evtimer_t my_evtimer;
    evtimer_mbox_event_t my_event;
    mbox_t my_mbox;
    msg_t my_msg_queue[8]; // message queue buffer

    // Initialize message box
    mbox_init(&my_mbox, my_msg_queue, sizeof(my_msg_queue));

    // Initialize the evtimer with the mbox handler
    evtimer_init_mbox(&my_evtimer);

    // Set up the event
    my_event.event.callback = event_callback;
    my_event.event.periodic = true;
    my_event.event.arg = NULL;  // No arguments passed
    my_event.event.offset = SLEEP_INTERVAL;

    // Add the event to the event timer
    evtimer_add_mbox(&my_evtimer, &my_event, &my_mbox);

    while (true) {
        // Wait for the period
        xtimer_usleep(SLEEP_INTERVAL);

        // Dispatch any events that are ready
        evtimer_dispatch(&my_evtimer);
    }

    return 0;
}