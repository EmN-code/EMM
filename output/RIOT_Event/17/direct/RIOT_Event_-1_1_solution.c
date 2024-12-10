#include <ztimer.h>
#include <stdio.h>
#include <inttypes.h>

#define INTERVAL (1U * ZTIMER_MSEC_PER_SEC)

static ztimer_t timer;

void event_periodic(void *arg) {
    (void)arg;
    printf("Callback function called every second!\n");
    ztimer_set(ZTIMER_MSEC, &timer, INTERVAL);
}

int main(void) {
    printf("Starting periodic event example on ESP32\n");
    
    // Initialize the timer
    ztimer_init();
    
    // Set the timer to call event_periodic for the first time
    timer.callback = event_periodic;
    ztimer_set(ZTIMER_MSEC, &timer, INTERVAL);
    
    while (1) {
        // The main loop can be used for other tasks
        // The event_periodic function will be called every second by the timer
    }
    return 0;
}