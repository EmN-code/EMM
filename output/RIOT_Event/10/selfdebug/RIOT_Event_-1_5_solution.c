#include <ztimer.h>
#include <stdio.h>

static ztimer_t timer;

void callback(void *arg) {
    puts("Event triggered");
}

void init_and_start_timer(void) {
    ztimer_set(&timer, ZTIMER_MSEC, ztimer_now(ZTIMER_MSEC) + 1000);
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
        callback(NULL);
    }
}

int main(void) {
    init_and_start_timer();
    return 0;
}