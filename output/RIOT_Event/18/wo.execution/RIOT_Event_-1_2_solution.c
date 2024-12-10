#include "ztimer.h"
#include "xtimer.h"
#include "timex.h"
#include "time_units.h"
#include "event/timeout.h"
#include "event/callback.h"
#include "event/thread.h"
#include "evtimer_msg.h"
#include "evtimer_mbox.h"

static void callback_function(void *arg) {
    (void)arg; // Avoid unused parameter warning
    printf("Callback function called every second!\n");
}

int main(void) {
    ztimer_t timer;
    ztimer_set(ZTIMER_MSEC, &timer, SEC_TO_MS(1));
    ztimer_callback_add(ZTIMER_MSEC, &timer, (void *)callback_function, NULL);

    while (1) {
        event_thread_get((event_callback_t*) &timer.thread)->callback(NULL);
        xtimer_sleep(1);
    }

    return 0;
}