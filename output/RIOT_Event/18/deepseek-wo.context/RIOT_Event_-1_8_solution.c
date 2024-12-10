
#include "xtimer.h"
#include "event/callback.h"

#define TIMER_INTERVAL (1 * 1000 * 1000U)  // 1 second in microseconds

static void periodic_callback(void *arg) {
    (void)arg;  // Unused argument
    printf("Periodic callback triggered\n");
}

int main(void) {
    xtimer_t timer;
    event_callback_t event_cb;

    // Initialize the event callback structure
    event_cb.handler = (event_callback_handler_t)periodic_callback;
    event_cb.arg = NULL;

    // Initialize the timer
    xtimer_init(&timer);

    // Set the timer to trigger the callback every second
    xtimer_set_periodic(&timer, TIMER_INTERVAL, (xtimer_callback_t)event_cb.handler, event_cb.arg);

    // Main loop to keep the application running
    while (1) {
        xtimer_usleep(1000000);  // Sleep for 1 second
    }

    return 0;
}
