
#include <stdlib.h>
#include <mbox.h>
#include <evtimer_mbox.h>
#include <stdio.h>
#include <msg.h>
#include <ztimer.h>
#include <stdatomic.h>
#include <unistd.h>
#include <timex.h>
#include <stddef.h>
#include <stdbool.h>
#include <time_units.h>
#include <xtimer.h>
#include <evtimer_msg.h>

#define PERIODIC_INTERVAL (1 * US_PER_SEC)  // 1 second interval

static evtimer_t evtimer;
static msg_t msg_queue[8];

void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    // Initialize the message queue
    msg_init_queue(msg_queue, sizeof(msg_queue) / sizeof(msg_queue[0]));

    // Initialize the event timer for mbox events
    evtimer_init_mbox(&evtimer);

    // Set up the periodic event
    evtimer_mbox_event_t event;
    event.event.handler = periodic_callback;
    event.event.arg = NULL;

    // Add the event to the event timer with a 1-second interval
    evtimer_add_mbox(&evtimer, &event, PERIODIC_INTERVAL);

    // Main loop to handle messages
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        // Handle messages if needed
    }

    return 0;
}
