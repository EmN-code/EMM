#include "mqtt/MQTTClient.h"
#include "event/event.h"
#include "event/callback.h"
#include "event/thread.h"
#include "ztimer.h"
#include <stdio.h>

#define TIMER_INTERVAL (1U * US_PER_SEC)  // Define the timer interval as 1 second in microseconds

static ztimer_t timer;

void timer_callback(void *arg) {
    (void)arg;  // Suppress unused parameter warning
    printf("Timer callback executed\n");
}

int main(void) {
    puts("RIOT Timer Callback Example\n");

    // Initialize the ztimer_t structure
    timer.callback = timer_callback;
    timer.arg = NULL;

    // Set and start the timer
    ztimer_set(&timer, TIMER_INTERVAL);

    // Run the event loop
    while (1) {
        // Simulate an infinite loop to keep the program running
    }

    return 0;  // Although this code will never reach here
}