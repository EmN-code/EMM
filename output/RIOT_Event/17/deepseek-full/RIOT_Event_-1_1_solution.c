
#include <stdio.h>
#include "thread.h"
#include "xtimer.h"
#include "event/periodic.h"

#define MAIN_QUEUE_SIZE     (8)
static msg_t _main_msg_queue[MAIN_QUEUE_SIZE];

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    // Initialize the main message queue
    msg_init_queue(_main_msg_queue, MAIN_QUEUE_SIZE);

    // Create an event timer
    evtimer_t evtimer;
    evtimer_init(&evtimer, NULL);

    // Set up a periodic event
    evtimer_periodic_event_t periodic_event;
    evtimer_periodic_event_init(&periodic_event, periodic_callback, NULL, 1000000U); // 1 second interval

    // Add the periodic event to the event timer
    evtimer_add_periodic(&evtimer, &periodic_event);

    // Main loop to keep the application running
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        // Handle messages if needed
    }

    return 0;
}
