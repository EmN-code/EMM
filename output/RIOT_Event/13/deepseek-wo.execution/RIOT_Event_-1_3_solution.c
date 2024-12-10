
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <timex.h>
#include <stddef.h>
#include <stdbool.h>
#include <time_units.h>
#include <xtimer.h>
#include <evtimer_msg.h>
#include <msg.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <mbox.h>
#include <evtimer_mbox.h>

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static evtimer_t evtimer;
static msg_t msg_queue[8];

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize the event timer with the message handler
    evtimer_init_msg(&evtimer);

    // Create an event for the periodic callback
    evtimer_event_t event;
    event.event.handler = periodic_callback;
    event.event.arg = NULL;

    // Add the event to the event timer with the specified interval
    evtimer_add_msg(&evtimer, &event, PERIODIC_INTERVAL);

    // Main loop to keep the application running
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        // Handle messages if needed
    }

    return 0;
}
