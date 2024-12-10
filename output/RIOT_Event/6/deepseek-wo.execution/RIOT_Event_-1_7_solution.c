
#include "thread.h"
#include "evtimer.h"
#include "periph/timer.h"

#define TIMER_INTERVAL (1000000U)  // 1 second in microseconds

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
    event.event.handler = periodic_callback;
    event.event.arg = NULL;

    // Add the event to the event timer with a 1-second interval
    evtimer_add_msg(&evtimer, &event, thread_getpid());

    // Set the timer to trigger the event every second
    timer_set_periodic(0, TIMER_INTERVAL, (timer_cb_t)evtimer_add_msg, &evtimer);

    // Main loop to keep the application running
    while (1) {
        thread_yield();
    }

    return 0;
}
