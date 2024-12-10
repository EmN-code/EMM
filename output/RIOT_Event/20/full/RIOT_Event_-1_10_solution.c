#include "xtimer.h"
#include "evtimer_msg.h"
#include "stdio.h"
#include "stdlib.h"

static void _event_handler(void *arg)
{
    (void)arg;
    puts("Event triggered!");
}

int main(void)
{
    // Initializes the event timer
    evtimer_msg_event_t event;
    evtimer_t evtimer;

    // Initializes the event to trigger every second
    evtimer_msg_event_init(&event, &evtimer, _event_handler, NULL);
    evtimer_msg_event_set(&event, 1 * XTIMER_SEC);

    // Starts the event timer
    evtimer_msg_init(&evtimer);
    evtimer_msg_add(&evtimer, &event);

    // Simple loop to keep program running
    while (1) {
        xtimer_sleep(2);
    }
    return 0;
}