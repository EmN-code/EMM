#include <xtimer.h>
#include <stdio.h>
#include <event/periodic.h>
#include <event/callback.h>

void callback(void *arg) {
    printf("Periodic callback triggered!\n");
}

int main(void) {
    evtimer_t evtimer;
    evtimer_periodic_init(&evtimer, callback, NULL, 1 * US_PER_SEC);
    evtimer_start_periodic(&evtimer);
    
    while (1) {
        xtimer_sleep(2); // Sleep for a while to let the periodic event run
    }

    return 0;
}