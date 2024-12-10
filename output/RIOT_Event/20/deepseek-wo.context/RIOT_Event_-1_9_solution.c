
#include "xtimer.h"
#include "thread.h"
#include "periph/gpio.h"

#define TIMER_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

void event_periodic(void *arg) {
    (void)arg;  // Unused parameter
    printf("Event triggered every second\n");
}

int main(void) {
    xtimer_t timer;
    timer.callback = event_periodic;
    timer.arg = NULL;

    xtimer_set(&timer, TIMER_INTERVAL);

    while (1) {
        // Main loop
    }

    return 0;
}
