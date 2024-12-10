#include <xtimer.h>
#include <timex.h>
#include <stdio.h>

#define PERIODIC_INTERVAL (1 * US_PER_SEC)

void cb(void *arg) {
    (void)arg;
    puts("Periodically called every second.");
}

int main(void) {
    xtimer_ticks32_t last_wakeup = xtimer_now();

    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, PERIODIC_INTERVAL);
        cb(NULL);
    }

    return 0;
}