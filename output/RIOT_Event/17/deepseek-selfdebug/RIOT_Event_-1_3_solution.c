
#include "xtimer.h"
#include "thread.h"
#include "periph/gpio.h"

#define TIMER_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

static void event_periodic_callback(void *arg) {
    (void)arg;  // Unused argument
    printf("Event periodic callback triggered\n");
}

int main(void) {
    xtimer_t timer;

    // Initialize the timer
    xtimer_init(&timer);

    // Set the timer to trigger the callback every second
    xtimer_set_periodic(&timer, TIMER_INTERVAL, event_periodic_callback, NULL);

    // Keep the main thread alive
    while (1) {
        thread_yield();
    }

    return 0;
}
