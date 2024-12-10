
#include "xtimer.h"
#include "msg.h"
#include "evtimer_msg.h"

#define PERIODIC_INTERVAL (1000000U)  // 1 second in microseconds

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    evtimer_t evtimer;
    evtimer_event_t event;

    // Initialize the event timer
    evtimer_init_msg(&evtimer);

    // Set up the periodic event
    event.event.handler = periodic_callback;
    event.event.arg = NULL;

    // Schedule the event to occur every second
    xtimer_t timer;
    timer.target = xtimer_now_usec() + PERIODIC_INTERVAL;
    timer.long_target = timer.target;
    timer.callback = (xtimer_callback_t)evtimer_add;
    timer.arg = &event;

    // Start the periodic timer
    xtimer_set(&timer, PERIODIC_INTERVAL);

    // Main loop to keep the application running
    while (1) {
        msg_t msg;
        msg_receive(&msg);
    }

    return 0;
}
