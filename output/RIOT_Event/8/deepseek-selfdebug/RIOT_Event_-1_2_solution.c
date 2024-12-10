
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define PERIODIC_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

static void periodic_callback(void *arg) {
    (void)arg;
    printf("Periodic event triggered!\n");
}

int main(void) {
    xtimer_t timer;
    event_t event;

    event_init(&event, periodic_callback, NULL);
    xtimer_init(&timer, PERIODIC_INTERVAL, &event);

    while (1) {
        xtimer_periodic_wakeup(&timer, PERIODIC_INTERVAL);
    }

    return 0;
}
