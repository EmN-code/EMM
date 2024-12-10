#include <xtimer.h>
#include <stdio.h>

#define INTERVAL (1000000U) // 1 second in microseconds

typedef struct {
    xtimer_t timer;
    uint32_t interval;
} periodic_event_t;

void event_periodic(void *arg) {
    periodic_event_t *evt = (periodic_event_t *)arg;
    printf("Event triggered!\n");
    xtimer_set(&evt->timer, evt->interval);
}

int main(void) {
    periodic_event_t evt;
    evt.interval = INTERVAL;
    evt.timer.callback = event_periodic;
    evt.timer.arg = &evt;

    printf("Starting periodic event timer.\n");
    xtimer_set(&evt.timer, evt.interval);

    // Normal operation of your program
    while (1) {
        // The main loop can perform other tasks
        xtimer_sleep(1);
    }

    return 0;
}