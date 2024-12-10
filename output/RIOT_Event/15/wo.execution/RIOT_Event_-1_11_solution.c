#include <ztimer.h>
#include <stdio.h>
#include <event.h>

// Callback function that will execute every second
typedef struct {
    ztimer_t timer;
} periodic_t;

void periodic_callback_handler(void *arg) {
    printf("Periodic event triggered\n");
}

int main(void) {
    // Define and initialize the periodic event
    periodic_t periodic_timer;
    ztimer_periodic_init(ZTIMER_MSEC, &periodic_timer.timer, periodic_callback_handler, NULL, 1000);
    // Start the periodic timer with 1 second intervals
    ztimer_periodic_start(&periodic_timer.timer);

    // Keep the main thread alive
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}