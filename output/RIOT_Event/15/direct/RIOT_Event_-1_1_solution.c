#include <stdio.h>
#include "xtimer.h"
#include "timex.h"

#define INTERVAL (1U * US_PER_SEC)

void callback(void *arg) {
    (void)arg; // Avoid unused parameter warning
    puts("Event periodic!");
}

int main(void) {
    xtimer_t timer;
    timer.callback = callback;
    timer.arg = NULL;
    timer.target = xtimer_now() + INTERVAL;
    timer.long_target = 0;

    while (1) {
        xtimer_set(&timer, INTERVAL);
        xtimer_periodic_wakeup(&timer.target, INTERVAL);
    }

    return 0;
}