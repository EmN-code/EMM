#include "unistd.h"
#include "stddef.h"
#include "stdio.h"
#include "stdbool.h"
#include "stdlib.h"
#include "xtimer.h"

#include "evtimer.h"

#define INTERVAL (1U * 1000000U) // 1 second in microseconds

static void my_event_callback(void *arg)
{
    puts("Event triggered");
    (void)arg;
}

int main(void)
{
    evtimer_t my_timer;
    evtimer_event_t my_event;
    
    // Initialize event timer
    evtimer_init(&my_timer, my_event_callback, NULL);
    
    // Set periodic event
    evtimer_set(&my_event, INTERVAL);
    
    // Add event to timer
    evtimer_add(&my_timer, &my_event);
    
    // Run the event timer
    evtimer_run(&my_timer);
    
    return 0;
}
