#include <ztimer.h>  // Include the ztimer interface
#include <xtimer.h>  // XTimer for delay and scheduling
#include <stdio.h>   // Standard input/output library
#include <msg.h>     // Message passing interface in RIOT

#define TIMER_INTERVAL (1000000U)  // Set timer interval to 1 second (in microseconds)

static ztimer_t my_timer; // Declare a ztimer_t variable

void timer_callback(void *arg) {
    (void)arg; // Remove unused parameter warning
    printf("Timer callback triggered\n");
}

int main(void) {
    // Initialize the timer (my_timer), set the callback function and argument
    my_timer.callback = timer_callback;
    my_timer.arg = NULL;

    // Start the timer with a periodic interval
    ztimer_set(ZTIMER_MSEC, &my_timer, TIMER_INTERVAL);
    
    // The main loop to keep the program running
    while (1) {
        // Here you could put any other operations for the main loop
        ztimer_sleep(ZTIMER_MSEC, TIMER_INTERVAL); // Sleep for the timer interval
    }
    
    return 0;
}