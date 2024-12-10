
#include "xtimer.h"
#include "evtimer.h"
#include "thread.h"
#include "msg.h"

#define TIMER_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

static evtimer_t evtimer;
static evtimer_msg_event_t event;

void event_periodic_callback(void *arg)
{
    (void)arg;
    printf("Event periodic callback triggered!\n");
}

int main(void)
{
    // Initialize the event timer
    evtimer_init(&evtimer);

    // Initialize the event
    evtimer_event_init(&event.event, event_periodic_callback, NULL, TIMER_INTERVAL);

    // Add the event to the event timer
    evtimer_add_event(&evtimer, &event.event);

    // Main loop to keep the thread alive
    while (1) {
        msg_t msg;
        msg_receive(&msg);
    }

    return 0;
}
