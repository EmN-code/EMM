#include <ztimer.h>
#include <stdio.h>
#include <event.h>

#define INTERVAL (1U * US_PER_SEC) // 1 second interval

static void _callback(void *arg) {
    (void)arg; // Ignore unused parameter warning
    puts("Callback function executed.");
}

int main(void) {
    ztimer_t timer;

    // Initialize the periodic timer
    ztimer_set_msg(&timer, ZTIMER_MSEC, (ztimer_cb_t)_callback, NULL);

    // Start the periodic timer
    while (1) {
        ztimer_periodic_wakeup(ZTIMER_MSEC, INTERVAL);
    }

    return 0;
}