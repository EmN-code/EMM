#include <xtimer.h>  // Event Timer header
#include <event/callback.h>  // For event callback
#include <evtimer.h> // Event timer related functions

#define TIMER_INTERVAL (1U * US_PER_SEC)  // Set timer interval to 1 second

static evtimer_t evtimer;
static evtimer_event_t e_event;

void callback(void *arg) {
    (void)arg;
    // Placeholder callback function
    puts("Timer callback executed!");
}

static void init_event_periodic(void) {
    // Initialize evtimer with callback
    evtimer_init(&evtimer, evtimer_callback, NULL);
    // Set evtimer for periodic execution every second
    evtimer_periodic(&evtimer, TIMER_INTERVAL);
    // Add event to evtimer
    evtimer_add(&evtimer, &e_event, TIMER_INTERVAL, callback, NULL);
}

int main(void) {
    puts("Starting event periodic example with an ESP32 and RIOT OS.");
    // Initialize the event callback function to be called periodically
    init_event_periodic();
    // Keep the thread alive so we can keep calling the timer
    while (1) {
        xtimer_sleep(1);
    }
    return 0;
}

/*
In this code:
1. Added the necessary includes following the correct header order.
2. Defined the TIMER_INTERVAL for 1 second periodicity.
3. Initialized the evtimer and set the periodicity using 'evtimer_periodic'.
4. Created a callback function that executes on each timer tick.
5. Used 'evtimer_add' to add periodic events to our evtimer.
6. Maintained the main loop to prevent application close, using 'xtimer_sleep'.
*/
