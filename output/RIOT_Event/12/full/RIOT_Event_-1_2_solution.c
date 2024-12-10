#include <ztimer.h>
#include <timex.h>
#include <stdio.h>

#define INTERVAL (1U * US_PER_SEC)

static void _callback(void *arg) {
    (void)arg;
    printf("Periodic callback triggered!\n");
}

int main(void) {
    ztimer_t timer;
    timer.callback = _callback;
    timer.arg = NULL;

    while (1) {
        ztimer_set(ZTIMER_MSEC, &timer, INTERVAL);
        ztimer_sleep(ZTIMER_MSEC, INTERVAL);
    }
    return 0;
}