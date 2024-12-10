#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "thread.h"
#include "time_units.h"
#include "event/thread.h"
#include "event/callback.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

static void timer_callback(void *arg) {
    (void)arg;  // Unused parameter
    printf("Timer event triggered\n");
}

int main(void) {
    ztimer_t timer;
    ztimer_callback_t callback;
    callback.func = timer_callback;
    callback.arg = NULL;

    // Initialize a periodic timer to trigger every 1 second
    ztimer_set_periodic(ZTIMER_MSEC, &timer, 1000, callback);

    while(1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);  // Sleep for 1 second before checking again
    }

    return 0;
}