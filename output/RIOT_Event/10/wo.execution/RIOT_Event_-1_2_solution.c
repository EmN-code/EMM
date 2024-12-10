#include "ztimer.h"
#include "xtimer.h"
#include "time_units.h"
#include "timex.h"
#include "thread.h"
#include "evtimer_mbox.h"
#include "evtimer_msg.h"
#include "event/thread.h"

#define TIMER_INTERVAL (1U * US_PER_SEC)  // 1 second interval

static ztimer_t timer;

// Callback function to be called by the timer
static void event_periodic(void *arg) {
    (void)arg; // Avoid unused parameter warning
    // Insert your periodic task here
    printf("Event Triggered every second!\n");
    ztimer_set(&ztimer_sec, &timer, TIMER_INTERVAL); // Reset the timer
}

int main(void) {
    // Initialize the timer with the default ztimer second precision
    ztimer_init();
    
    // Set the callback function for the timer
    timer.callback = event_periodic;
    timer.arg = NULL;

    // Set the timer to call the function after the interval
    ztimer_set(&ztimer_sec, &timer, TIMER_INTERVAL);

    while (1) {
        // Main thread can perform other tasks or simply idle
        ztimer_sleep(ZTIMER_SEC, TIMER_INTERVAL); // Sleep for the interval time to save energy
    }

    return 0;
}