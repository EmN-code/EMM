
#include <stdio.h>
#include "thread.h"
#include "xtimer.h"
#include "event/periodic.h"

#define MAIN_THREAD_STACKSIZE    (THREAD_STACKSIZE_DEFAULT)
#define MAIN_THREAD_PRIO         (THREAD_PRIORITY_MAIN - 1)

static void periodic_callback(void *arg)
{
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void)
{
    // Initialize the event timer
    evtimer_t evtimer;
    evtimer_init(&evtimer);

    // Set up the periodic event
    evtimer_periodic_t periodic_event;
    evtimer_periodic_init(&periodic_event, periodic_callback, NULL);

    // Add the periodic event to the event timer with a 1-second interval
    evtimer_add_periodic(&evtimer, &periodic_event, 1000000U);

    // Main loop to keep the thread alive
    while (1) {
        thread_yield();
    }

    return 0;
}
