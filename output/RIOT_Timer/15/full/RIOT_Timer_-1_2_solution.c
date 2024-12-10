#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

#define INTERVAL 1000000  // Interval in microseconds for the timer

void timer_callback(void *arg) {
    (void)arg;
    // Logic to turn on the LED
    printf("LED Toggled\n");
}

int main(void) {
    // Initialize ztimer
    ztimer_t timer;

    // Setup timer with 1 second interval
    ztimer_set(ZTIMER_USEC, &timer, INTERVAL);

    // Assign the callback function
    timer.callback = timer_callback;

    while(1) {
        // Sleep for a second in each iteration to simulate timer firing
        ztimer_wait(ZTIMER_USEC, INTERVAL);

        // Call the callback function manually (since we're simulating behavior)
        timer.callback(NULL);
    }

    return 0;
}