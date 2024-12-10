
#include <ztimer.h>
#include <xtimer.h>
#include <stdio.h>
#include <stdbool.h>
#include "evtimer_mbox.h"

#define PERIOD (1U * US_PER_SEC)

static evtimer_t evtimer;
static xtimer_t timer;

static void timer_callback(void *arg) {
    (void)arg;
    printf("Timer event every second!\n");
}

int main(void) {
    printf("Starting periodic event timer\n");
    
    evtimer_init_msg(&evtimer);
    
    timer.callback = timer_callback;
    xtimer_set(&timer, PERIOD);

    while (1) {
        xtimer_periodic_wakeup(PERIOD);
        timer_callback(NULL);
    }

    return 0;
}