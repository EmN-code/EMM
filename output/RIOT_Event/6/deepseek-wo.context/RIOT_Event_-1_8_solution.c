
#include <stdio.h>
#include "xtimer.h"
#include "thread.h"
#include "msg.h"
#include "evtimer.h"

#define PERIODIC_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

static evtimer_t periodic_timer;
static msg_t periodic_msg;

void periodic_callback(void *arg)
{
    printf("Periodic event triggered!\n");
}

int main(void)
{
    // Initialize the periodic timer
    evtimer_init(&periodic_timer);

    // Initialize the message for the periodic event
    periodic_msg.type = 0;
    periodic_msg.content.ptr = NULL;

    // Create an event for the periodic timer
    evtimer_event_t periodic_event;
    periodic_event.callback = periodic_callback;
    periodic_event.arg = NULL;

    // Add the periodic event to the timer
    evtimer_add(&periodic_timer, &periodic_event, PERIODIC_INTERVAL);

    // Main loop to keep the application running
    while (1) {
        msg_t msg;
        msg_receive(&msg);
    }

    return 0;
}
