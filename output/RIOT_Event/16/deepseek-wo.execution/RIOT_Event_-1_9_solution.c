
#include "xtimer.h"
#include "thread.h"
#include "msg.h"
#include "evtimer_msg.h"
#include "stdio.h"
#include "stdlib.h"

#define PERIODIC_INTERVAL (1000000U)  // 1 second in microseconds

static evtimer_t evtimer;
static evtimer_msg_event_t event;

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    // Initialize the event timer
    evtimer_init_msg(&evtimer);

    // Set up the event
    event.event.callback = periodic_callback;
    event.event.arg = NULL;

    // Add the event to the timer with a 1-second interval
    evtimer_add_msg(&evtimer, &event, thread_getpid());

    // Main loop to keep the thread alive
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        // Handle the message if needed
    }

    return 0;
}
