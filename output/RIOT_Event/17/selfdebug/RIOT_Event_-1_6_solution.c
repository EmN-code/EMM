#include <ztimer.h>
#include <stdio.h>

#define TIMER_INTERVAL (1U * US_PER_SEC)  // 1 second interval

static ztimer_t event_timer;

void event_periodic(void *arg) {
    (void)arg;
    printf("Periodic callback triggered!\n");
    ztimer_set(ZTIMER_MSEC, &event_timer, TIMER_INTERVAL);
}

int main(void) {
    printf("Starting periodic timer example on ESP32\n");

    // Initialize the timer
    ztimer_set(ZTIMER_MSEC, &event_timer, TIMER_INTERVAL);
    event_timer.callback = event_periodic;
    event_timer.arg = NULL;

    // Start the periodic event
    event_periodic(NULL);

    // Main loop
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, TIMER_INTERVAL);
    }

    return 0;
}