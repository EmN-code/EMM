#include <ztimer.h>
#include <stdio.h>

#define TIMER_INTERVAL (1U * 1000000U) // Timer interval of 1 second

static ztimer_t timer;

void timer_callback(void *arg) {
    (void)arg;
    printf("LED ON\n");
    // Code to turn on LED goes here
}

int main(void) {
    printf("Starting timer example\n");
    ztimer_set(&timer, TIMER_INTERVAL);
    timer.callback = timer_callback;
    ztimer_periodic_wakeup(ZTIMER_SEC, 1);

    return 0;
}