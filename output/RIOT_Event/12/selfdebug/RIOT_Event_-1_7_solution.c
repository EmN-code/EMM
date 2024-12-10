#include "ztimer.h"
#include "timex.h"
#include "event.h"
#include "stdio.h"

#define TIMER_INTERVAL (1U * US_PER_SEC)

static void on_timeout(void *arg) {
    (void)arg;
    printf("Timer tick\n");
}

int main(void) {
    ztimer_t my_timer;
    ztimer_periodic_init(ZTIMER_MSEC, &my_timer, on_timeout, NULL, TIMER_INTERVAL);

    while (1) {
        ztimer_periodic_wait(&my_timer);
    }

    return 0;
}