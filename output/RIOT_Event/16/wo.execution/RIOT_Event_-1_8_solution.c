#include <xtimer.h>
#include <ztimer.h>
#include <stdbool.h>
#include <stdlib.h>
#include <event/callback.h>
#include <net/sock/udp.h>
#include <net/emcute.h>
#include <event/thread.h>

#define INTERVAL_US (1U * US_PER_SEC)

static void evtimer_callback(void *arg) {
    (void)arg;
    printf("Periodic event triggered\n");
}

int main(void) {
    evtimer_t evtimer;
    evtimer.timer.callback = evtimer_callback;
    evtimer.timer.arg = NULL;
    
    xtimer_periodic_wakeup(&(evtimer.base), INTERVAL_US);
    
    while (1) {
        // Sleep interval
        ztimer_sleep(ZTIMER_USEC, INTERVAL_US);
        // Trigger the callback
        evtimer.callback(evttimer.events);
    }
    return 0;
}