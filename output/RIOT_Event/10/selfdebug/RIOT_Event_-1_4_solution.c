#include <ztimer.h>
#include <stdio.h>

#define US_PER_SEC (1000000UL)  // Define microseconds per second

void event_periodic(void *arg) {
    (void)arg;  // Unused parameter
    printf("Periodic event called!\n");
}

int main(void) {
    ztimer_t timer;
    
    // Initialize the timer and set it to tick every second
    ztimer_set_msg(ZTIMER_SEC, &timer, US_PER_SEC, &event_periodic, NULL);

    // Start the periodic timer
    ztimer_start_periodic(ZTIMER_SEC, &timer);

    while(1) {
        // Main loop can perform other tasks
        // The callback function will be periodically called every second
    }

    return 0;
}