#include <xtimer.h>
#include <stdio.h>

void callback(void *arg) {
    (void)arg;
    puts("Callback executed");
}

int main(void) {
    xtimer_ticks32_t interval = xtimer_ms_to_ticks32(1000);
    xtimer_t timer;

    timer.callback = callback;
    timer.arg = NULL;

    xtimer_set(&timer, interval);

    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}