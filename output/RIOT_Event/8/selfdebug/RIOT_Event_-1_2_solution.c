#include <xtimer.h>  // Include xtimer for timer functionality
#include <stdio.h>    // Include standard input/output library
#include <msg.h>      // Include for messaging API

#define TIMER_INTERVAL (1U * US_PER_SEC)  // Set timer interval to 1 second

static void event_periodic(void *arg)
{
    (void)arg;  // Suppress unused variable warning
    printf("Callback triggered every second!\n");
}

int main(void)
{
    msg_t msg;
    xtimer_ticks32_t last_wakeup = xtimer_now();
    
    while (1) {
        // Initialize and start periodic timer with 1-second interval
        xtimer_periodic_wakeup(&last_wakeup, TIMER_INTERVAL);
        event_periodic(NULL);  // Call the callback function
    }

    return 0;
}