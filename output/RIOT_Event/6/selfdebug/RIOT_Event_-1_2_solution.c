#include <ztimer.h>
#include <stdio.h>

#define TIMER_INTERVAL (1 * US_PER_SEC)

static void my_periodic_callback(void *arg) {
    (void)arg;
    puts("Periodic callback executed");
}

int main(void) {
    ztimer_periodic_t periodic_timer;
    ztimer_periodic_init(ZTIMER_USEC, &periodic_timer, my_periodic_callback, NULL, TIMER_INTERVAL);
    ztimer_periodic_start(&periodic_timer);

    while (1) {
        ztimer_sleep(ZTIMER_USEC, TIMER_INTERVAL);
    }
    return 0;
}