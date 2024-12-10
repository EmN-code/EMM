#include <ztimer.h>
#include <stdio.h>  // for the puts function

void event_periodic(void *arg) {
    (void)arg;
    puts("Callback triggered");
}

int main(void) {
    puts("Starting periodic timer example...");
    
    // Set up the periodic timer
    ztimer_periodic_t periodic_timer;
    ztimer_periodic_init(&periodic_timer, ZTIMER_SEC, event_periodic, NULL, ztimer_usec_from_ms(1000));
    ztimer_periodic_start(&periodic_timer);

    // Main loop
    while (1) {
        // Add sleep to simulate idle state
        ztimer_sleep(ZTIMER_SEC, 1);
    }
    return 0;
}