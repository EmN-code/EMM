#include <ztimer.h>
#include <timex.h>
#include <stdio.h>

#define TIMER_INTERVAL (1U * US_PER_SEC)  // One second in microseconds

ztimer_t event_periodic_timer;

void event_callback(void *arg) {
    (void)arg;
    puts("Event triggered every second");
    ztimer_set(ZTIMER_MSEC, &event_periodic_timer, TIMER_INTERVAL);
}

int main(void) {
    puts("Starting event_periodic example on ESP32...");

    // Initialize and set the timer for the first time.
    ztimer_set(ZTIMER_MSEC, &event_periodic_timer, TIMER_INTERVAL);
    ztimer_periodic_cb_init(&event_periodic_timer, TIMER_INTERVAL, event_callback, NULL);

    // Simulate blocking behavior to keep the program running
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, TIMER_INTERVAL);
    }

    return 0;
}