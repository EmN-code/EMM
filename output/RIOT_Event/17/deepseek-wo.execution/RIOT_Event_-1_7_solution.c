
#include <stdlib.h>
#include <stdio.h>
#include <xtimer.h>

#define TIMER_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

void event_periodic_callback(void) {
    printf("Event periodic callback triggered!\n");
}

int main(void) {
    xtimer_t timer;
    uint32_t last_wakeup = xtimer_now().ticks32;

    while (1) {
        xtimer_periodic_wakeup(&last_wakeup, TIMER_INTERVAL);
        event_periodic_callback();
    }

    return 0;
}
