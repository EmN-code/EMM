#include <ztimer.h>
#include <stdio.h>

static ztimer_t timer;

void callback(void *arg) {
    (void)arg;
    puts("Timer callback triggered");
    ztimer_set(ZTIMER_MSEC, &timer, 1000);
}

int main(void) {
    printf("Initializing timer\n");
    ztimer_set(ZTIMER_MSEC, &timer, 1000);
    timer.callback = callback;
    timer.arg = NULL;
    return 0;
}