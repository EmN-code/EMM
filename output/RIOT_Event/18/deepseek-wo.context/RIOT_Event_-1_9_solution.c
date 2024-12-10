
#include "xtimer.h"
#include "evtimer.h"
#include "msg.h"

#define TIMER_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

static evtimer_t evtimer;
static evtimer_msg_event_t event;

void event_periodic_callback(void *arg)
{
    (void)arg;  // Unused parameter
    printf("Event periodic callback triggered\n");
}

int main(void)
{
    // Initialize the event timer
    evtimer_init_msg(&evtimer);

    // Set up the event
    event.event.callback = (evtimer_callback_t)event_periodic_callback;
    event.event.arg = NULL;

    // Add the event to the event timer
    evtimer_add_msg(&evtimer, &event, 0);

    // Set the timer to trigger the event every second
    xtimer_set(&event.event.timer, TIMER_INTERVAL);

    // Main loop to keep the application running
    while (1) {
        msg_t msg;
        msg_receive(&msg);
    }

    return 0;
}
