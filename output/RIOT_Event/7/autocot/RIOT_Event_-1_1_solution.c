#include <xtimer.h>
#include <stdio.h>
#include <event/periodic.h>

#define INTERVAL (1U * 1000000U) // 1 second expressed in microseconds

static void periodic_callback(void *arg) {
    (void) arg;  // avoid unused parameter warning
    printf("Periodic event triggered\n");
}

int main(void) {
    evtimer_t periodic_timer;

    // Prepare periodic timer event with a callback
    evtimer_event_t event;
    event.callback = periodic_callback;
    event.arg = NULL;

    // Initialize event periodic
    evtimer_init_periodic(&periodic_timer);
    
    // Add the event and trigger every INTERVAL seconds
    evtimer_add_periodic(&periodic_timer, &event, INTERVAL);
    
    printf("Event periodic timer started\n");

    // Run indefinitely
    while (1) {
        xtimer_sleep(1);  // Let the timer handler do its job
    }

    return 0;
}