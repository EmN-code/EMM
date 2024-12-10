
#include "xtimer.h"
#include "thread.h"

#define TIMER_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

static void event_periodic_callback(void *arg) {
    (void)arg;  // Unused argument
    printf("Event periodic callback triggered\n");
}

int main(void) {
    xtimer_t timer;

    // Initialize the timer
    xtimer_init();

    // Set the periodic callback
    xtimer_periodic_wakeup(&timer, TIMER_INTERVAL, event_periodic_callback, NULL);

    // Keep the main thread alive
    while (1) {
        thread_yield();
    }

    return 0;
}
