#include "xtimer.h"
#include <stdio.h>
#include <stdlib.h>

#define INTERVAL_SEC (1U)  // 1-second interval

static void event_periodic(void *arg) {
    (void) arg;
    printf("Periodic event triggered\n");
}

int main(void) {
    xtimer_ticks32_t last_wakeup = xtimer_now();
   
    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, INTERVAL_SEC * US_PER_SEC);
        event_periodic(NULL);
    }
    return 0;
}