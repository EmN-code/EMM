#include <ztimer.h>
#include <stdio.h>

void event_periodic_callback(void *arg) {
    printf("Periodic event triggered\n");
}

int main(void) {
    // Declare a ztimer_periodic_t object
    ztimer_periodic_t periodic_timer;

    // Initialize periodic timer: every second
    ztimer_periodic_init(ZTIMER_MSEC, &periodic_timer, event_periodic_callback, NULL, 1000);

    // Start the periodic timer
    ztimer_periodic_start(&periodic_timer);

    // Run an infinite loop
    while (1) {
        // Optional: Low-power sleep or other operations can be managed here
    }

    return 0;
}