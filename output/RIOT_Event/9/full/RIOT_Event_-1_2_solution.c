#include <ztimer.h>
#include <xtimer.h>
#include <timex.h>
#include <time_units.h>
#include <evtimer_msg.h>
#include <evtimer_mbox.h>

// Callback function for the periodic timer
void event_periodic_callback(void *arg) {
    (void) arg; // Avoid compiler warning for unused variable
    puts("Event periodic callback triggered.");
}

int main(void) {
    // Setting the interval for 1 second
    const uint32_t interval = 1U * US_PER_SEC;

    // ZTIMER_MSEC is used here to configure a periodic timer
    static ztimer_t timer;
    ztimer_set(&timer, interval, event_periodic_callback, NULL);

    // Start the timer in a periodic mode
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, interval);
        event_periodic_callback(NULL);
    }

    return 0;
}