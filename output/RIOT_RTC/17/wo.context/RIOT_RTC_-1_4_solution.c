#include <stdio.h>
#include <xtimer.h>
#include <event/callback.h>
#include <periph/cpu.h>

void alarm_callback(void *arg) {
    (void)arg;  // To silence the unused parameter warning.
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize alarm timer
    ztimer_t alarm_timer;
    ztimer_init(&alarm_timer);
    
    // Set up the timer to trigger after 5 seconds (5,000,000 microseconds)
    ztimer_set(ZTIMER_MSEC, &alarm_timer, 5000, alarm_callback, NULL);

    // Keep the program running to wait for the alarm
    while (1) {
        xtimer_sleep(10);  // Sleep to keep the program alive
    }

    return 0;
}