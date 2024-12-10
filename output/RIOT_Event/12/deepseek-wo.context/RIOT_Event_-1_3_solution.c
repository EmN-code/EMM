
#include "xtimer.h"
#include "event.h"
#include "thread.h"

#define PERIODIC_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

static void periodic_callback(void *arg) {
    (void)arg;  // Unused argument
    printf("Periodic event triggered!\n");
}

int main(void) {
    event_periodic_t periodic_event;
    event_periodic_init(&periodic_event, periodic_callback, NULL);
    event_periodic_add_periodic(&periodic_event, xtimer_ticks(PERIODIC_INTERVAL));

    while (1) {
        thread_yield();
    }

    return 0;
}
