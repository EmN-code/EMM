#include <xtimer.h>
#include <stdio.h>

// Constant for one second (in microseconds)
#define TIMER_PERIOD (1U * US_PER_SEC)

// Callback function that will be triggered every second
void event_periodic(void)
{
    printf("Timer callback function called.\n");
}

int main(void)
{
    printf("Periodic event setup starting.\n");
    
    // Initialize the timer
    xtimer_ticks32_t last_wakeup = xtimer_now();
    
    while (1) {
        // Wait to elapse the period
        xtimer_periodic_wakeup(&last_wakeup, TIMER_PERIOD);
        
        // Call the callback function
        event_periodic();
    }
    
    return 0;  // Although this line is never reached
}