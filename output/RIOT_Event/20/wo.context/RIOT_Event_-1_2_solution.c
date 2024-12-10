#include <xtimer.h>
#include <stdio.h>

#define TIMER_INTERVAL (1U * US_PER_SEC)  // 1 second interval

static void _callback(void *arg) {
    (void)arg;
    puts("Event periodic callback triggered");
}

int main(void) {
    xtimer_t timer;
    timer.callback = _callback;
    timer.arg = NULL;

    puts("Starting periodic event with 1 second interval");

    while (1) {
        xtimer_set(&timer, TIMER_INTERVAL);

        xtimer_usleep(TIMER_INTERVAL); // Sleep to keep the loop from running continuously
    }

    return 0;
}