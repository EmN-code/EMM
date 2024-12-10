#include "thread.h"
#include "event/thread.h"
#include "ztimer.h"
#include <stdio.h>

static void event_callback(void *arg) {
    (void)arg; // To handle unused parameter warning
    printf("Event triggered!\n");
}

int main(void) {
    ztimer_t timer;
    ztimer_periodic_init(&timer, ZTIMER_MSEC, 1000);  // Initialize a periodic timer with a 1-second interval

    while (1) {
        if (ztimer_periodic_interval_elapsed(&timer)) {
            event_callback(NULL);
            
            ztimer_periodic_init(&timer, ZTIMER_MSEC, 1000);  // Reset the timer
        }
    }
    return 0;
}