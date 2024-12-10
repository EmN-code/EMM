#include <ztimer.h>
#include <stdio.h>

#define INTERVAL (1U * ZTIMER_SEC) // Sets the interval to 1 second

void callback(void *arg) {
    (void)arg;
    puts("Callback executed.");
}

int main(void) {
    ztimer_periodic_t periodic_timer;
    ztimer_periodic_init(ZTIMER_MSEC, &periodic_timer, INTERVAL, callback, NULL);

    while (1) {
        ztimer_periodic_wait(&periodic_timer);
    }

    return 0;
}