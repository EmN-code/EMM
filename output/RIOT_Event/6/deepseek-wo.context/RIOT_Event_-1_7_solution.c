
#include "xtimer.h"
#include "evtimer.h"
#include "thread.h"
#include "msg.h"

#define PERIODIC_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

static evtimer_t periodic_timer;
static evtimer_msg_event_t periodic_event;

void periodic_callback(void *arg)
{
    (void)arg;  // Unused argument
    printf("Periodic callback triggered\n");
}

int main(void)
{
    // Initialize the event timer
    evtimer_init_msg(&periodic_timer);

    // Set up the periodic event
    periodic_event.event.callback = (evtimer_callback_t)periodic_callback;
    periodic_event.event.arg = NULL;

    // Add the event to the timer with a 1-second interval
    evtimer_add_msg(&periodic_timer, &periodic_event.event, PERIODIC_INTERVAL, thread_get_active_pid());

    // Main loop to keep the application running
    while (1) {
        msg_t msg;
        msg_receive(&msg);
        // Handle messages if needed
    }

    return 0;
}
