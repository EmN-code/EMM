#include "xtimer.h"
#include "evtimer.h"
#include <stdio.h>

#define INTERVAL (1000000) // 1 second in microseconds 

static void _callback(void *arg)
{
    (void)arg;
    printf("Callback triggered\n");
}

int main(void)
{   
    evtimer_t timer;
    evtimer_event_t event;

    // Initialize the event timer
    evtimer_init(&timer, evtimer_set_cb(&_callback, NULL));

    // Setup the event
    evtimer_set(&event, &timer, INTERVAL);

    // Start the event loop
    while (1) {
        evtimer_sleep(&event);
    }
    
    return 0;
}