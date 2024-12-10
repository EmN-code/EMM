#include <xtimer.h>
#include <stdio.h>

#define TIMER_INTERVAL (1000000U) // 1 second in microseconds

void timer_callback(void *arg) {
    (void)arg;
    printf("Timer callback executed\n");
}

int main(void) {
    xtimer_t timer;

    timer.callback = timer_callback;
    timer.arg = NULL;

    while (1) {
        xtimer_set(&timer, TIMER_INTERVAL);
        xtimer_usleep(TIMER_INTERVAL);
    }
    return 0;
}