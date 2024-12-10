
#include "xtimer.h"
#include "msg.h"
#include "evtimer_msg.h"
#include "kernel_defines.h"

#define PERIODIC_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

static evtimer_t periodic_timer;
static msg_t periodic_msg;

void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    // Initialize the message queue for the current thread
    msg_t msg_queue[8];
    msg_init_queue(msg_queue, 8);

    // Initialize the event timer
    evtimer_init_msg(&periodic_timer);

    // Create an event for the periodic timer
    evtimer_msg_event_t periodic_event;
    periodic_event.event.handler = (evtimer_callback_t)periodic_callback;
    periodic_event.event.arg = NULL;

    // Add the event to the timer with a 1-second interval
    xtimer_t periodic_xtimer;
    periodic_xtimer.target = xtimer_now_usec() + PERIODIC_INTERVAL;
    periodic_xtimer.long_target = (uint64_t)periodic_xtimer.target;
    periodic_xtimer.callback = (xtimer_callback_t)evtimer_add_msg;
    periodic_xtimer.arg = &periodic_event;

    // Start the periodic timer
    xtimer_set(&periodic_xtimer, PERIODIC_INTERVAL);

    // Main loop to handle messages
    while (1) {
        msg_receive(&periodic_msg);
    }

    return 0;
}
