#include "ztimer.h"
#include "xtimer.h"
#include "time_units.h"
#include "timex.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

#define INTERVAL_SEC 1

static void callback(void *arg) {
    (void)arg; // Avoid unused variable warning
    printf("Callback triggered every %d second(s)\n", INTERVAL_SEC);
}

int main(void) {
    ztimer_t timer;
    
    // Initialize the timer with the target time, callback, and argument (if any)
    ztimer_set_msg(ZTIMER_MSEC, &timer, callback, NULL);
    
    printf("Starting periodic timer with %d-second intervals\n", INTERVAL_SEC);
    
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, INTERVAL_SEC * MS_PER_SEC);
        // Manually trigger the callback, alternatively wait for the event in a real application
        callback(NULL);
    }
    return 0;
}