#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "stdio.h"
#include "stdlib.h"
#include "msg.h"
#include "evtimer_msg.h"

#define INTERVAL (1U * 1000U * 1000U) // 1 second in microseconds

static void event_periodic_callback(void *arg)
{
    (void)arg;
    puts("Periodic event triggered!");
}

int main(void)
{
    // Initialize the message queue
    msg_t queue[8];
    msg_init_queue(queue, 8);

    // Initializing event timer
    evtimer_msg_t evtimer;
    evtimer_event_t event;

    evtimer_init_msg(&evtimer);
    
    // Setting up the event
    event.msg.type = 0; // Dummy message type
    event.event.callback = event_periodic_callback;
    event.event.period = INTERVAL;

    // Adding the event to the timer
    evtimer_add_msg(&evtimer, &event, KERNEL_PID_MAIN);
    
    // Running the event timer
    while (1) {
        evtimer_run(&evtimer);
    }

    return 0;
}