#include <ztimer.h>
#include <stdio.h>
#include <event/callback.h>
#include "xtimer.h"

static void event_periodic(void *arg) {
    (void)arg;
    printf("Callback triggered\n");
}

int main(void) {
    ztimer_t timer;
    ztimer_periodic_t periodic;
    
    ztimer_periodic_init(ZTIMER_MSEC, &periodic, &timer, event_periodic, NULL, 1 * US_PER_SEC);
    
    while (1) {
        xtimer_sleep(1);
    }
    
    return 0;
}