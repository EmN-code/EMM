#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

#define INTERVAL (1U * US_PER_SEC)  // Timer interval in microseconds, equivalent to 1 second

void callback(void *arg) {
    (void)arg;
    // Print a message every second
    printf("Timer fired, performing periodic task\n");
}

int main(void) {
    // Initialize ztimer for periodic execution
    ztimer_t timer;
    ztimer_set_msg(&timer, ZTIMER_MSEC, ztimer_now(ZTIMER_MSEC) + INTERVAL, callback, NULL);

    while (1) {
        ztimer_sleep(ZTIMER_MSEC, INTERVAL);
    }

    return 0; // Should never actually reach this
}