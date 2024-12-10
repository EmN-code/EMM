#include <xtimer.h>
#include <stdio.h>

void callback(void *arg) {
    (void)arg; // Suppress unused variable warning
    printf("Callback triggered!\n");
}

int main(void) {
    xtimer_ticks32_t interval = xtimer_us_to_ticks(1000000); // 1 second interval
    xtimer_t timer;
    timer.callback = callback;
    timer.arg = NULL;

    while (1) {
        xtimer_set(&timer, interval);
        xtimer_sleep(1); // Sleep for 1 second
    }

    return 0;
}