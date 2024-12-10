
#include "xtimer.h"
#include "evtimer.h"
#include "msg.h"

#define PERIODIC_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

static evtimer_t periodic_timer;
static evtimer_msg_event_t periodic_event;

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    // Initialize the event timer
    evtimer_init_msg(&periodic_timer);

    // Initialize the event
    periodic_event.event.callback = periodic_callback;
    periodic_event.event.arg = NULL;
    periodic_event.msg.sender_pid = sched_active_pid;

    // Add the event to the event timer
    evtimer_add_msg(&periodic_timer, &periodic_event, sched_active_pid);

    // Set the periodic timer to trigger every second
    xtimer_t timer;
    timer.callback = (xtimer_callback_t)evtimer_add_msg;
    timer.arg = &periodic_event;
    xtimer_set(&timer, PERIODIC_INTERVAL);

    return 0;
}
