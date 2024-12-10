#include <xtimer.h>
#include <stdio.h>

#define INTERVAL (1000000)  // 1 second in microseconds

xtimer_t timer;

void timer_callback(void *arg) {
    (void)arg;
    printf("Timer triggered!\n");
    xtimer_set(&timer, INTERVAL);
}

int main(void) {
    printf("Starting periodic timer\n");
    timer.callback = timer_callback;
    timer.arg = NULL;
    xtimer_set(&timer, INTERVAL);
    return 0;
}