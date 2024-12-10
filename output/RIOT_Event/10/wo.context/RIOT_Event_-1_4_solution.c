#include <ztimer.h>   // For ztimer related functions
#include <stdio.h>     // For printf functionality
#include <evtimer.h>   // For event handling

#define MS_PER_SEC (1000U)
#define PRIO_DEFAULT (THREAD_PRIORITY_MAIN - 1)  // Define a default priority

static void my_event_callback(evtimer_event_t *event)
{
    printf("Event triggered\n");
}

int main(void)
{
    evtimer_t evtimer;
    evtimer_event_t event;
    evtimer_periodic_init(&evtimer, ZTIMER_MSEC, MS_PER_SEC, my_event_callback, &event);

    // Initialize the evtimer
    evtimer_init(&evtimer);

    // Set up a periodic event every second
    evtimer_event_init(&event, my_event_callback, NULL);
    evtimer_set_periodic(&evtimer, &event, MS_PER_SEC);

    // Main loop to dispatch events
    while (1) {
        evtimer_dispatch(&evtimer);
    }

    return 0;
}