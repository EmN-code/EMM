
#include "xtimer.h"
#include "evtimer.h"
#include "thread.h"
#include "mutex.h"
#include "msg.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdbool.h"
#include "kernel_defines.h"
#include "timex.h"
#include "stddef.h"
#include "mbox.h"

#define PERIODIC_INTERVAL (1000000U)  // 1 second in microseconds

static evtimer_t evtimer;
static evtimer_event_t periodic_event;

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
    evtimer_add(&evtimer, &periodic_event);
}

int main(void)
{
    // Initialize the event timer
    evtimer_init(&evtimer, periodic_callback, NULL);

    // Set up the periodic event
    periodic_event.event.offset = 0;
    periodic_event.event.callback = periodic_callback;
    periodic_event.event.arg = NULL;

    // Add the event to the timer
    evtimer_add(&evtimer, &periodic_event);

    // Main loop
    while (1) {
        xtimer_usleep(PERIODIC_INTERVAL);
    }

    return 0;
}
