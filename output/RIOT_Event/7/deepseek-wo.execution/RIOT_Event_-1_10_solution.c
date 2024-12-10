
#include "xtimer.h"
#include "thread.h"
#include "msg.h"
#include "evtimer_mbox.h"
#include "timex.h"
#include "stdbool.h"
#include "kernel_defines.h"
#include "stdio.h"

#define PERIODIC_INTERVAL (1000000U)  // 1 second in microseconds

static evtimer_t evtimer;
static evtimer_event_t periodic_event;

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void)
{
    // Initialize the event timer
    evtimer_init(&evtimer);

    // Set up the periodic event
    periodic_event.event.handler = periodic_callback;
    periodic_event.event.arg = NULL;

    // Add the event to the timer with a 1-second interval
    xtimer_t timer;
    timer.target = xtimer_now_usec();
    timer.long_target = timer.target + PERIODIC_INTERVAL;
    timer.callback = (xtimer_callback_t)evtimer_add;
    timer.arg = &periodic_event.event;

    // Set the timer to trigger the event periodically
    xtimer_set(&timer, PERIODIC_INTERVAL);

    // Main loop to keep the application running
    while (true) {
        xtimer_usleep(1000000);  // Sleep for 1 second
    }

    return 0;
}
