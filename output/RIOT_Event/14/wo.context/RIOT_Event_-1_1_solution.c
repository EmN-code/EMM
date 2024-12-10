#include <stdio.h>
#include <event/periodic.h>

#define TIMER_INTERVAL (1U * 1000000U) // 1 second in microseconds

void periodic_callback(void *arg) {
    (void)arg; // suppress unused variable warning
    printf("Periodic event triggered!\n");
}

int main(void) {
    evtimer_periodic_t periodic_timer;
    evtimer_periodic_init(&periodic_timer, TIMER_INTERVAL, periodic_callback, NULL);
    evtimer_periodic_start(&periodic_timer);
    
    // Main loop
    while (1) {
        // simulate doing other tasks
    }
    return 0;
}