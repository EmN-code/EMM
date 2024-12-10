#include <ztimer.h>
#include <stdio.h>
#include <event/periodic.h>

// Callback function that executes every second
event_periodic_t event;

event_periodic_callback_t _callback(void *arg) {
    (void)arg;  // Avoid unused parameter warning
    printf("Callback triggered!\n");
}

int main(void) {
    // Initialize event periodic structure
    event_periodic_init(&event, ZTIMER_SEC, _callback, NULL, 1);
    
    // Set the periodic event callback
event_periodic_start(&event);
    
    // Keep running indefinitely
    while (1) {
        ztimer_sleep(ZTIMER_SEC, 1);
    }

    return 0;
}