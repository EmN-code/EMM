#include <xtimer.h>     // for timers
#include <stdio.h>      // for input-output operations

// Define the alarm callback function
void rtc_alarm_callback(void *arg) {
    (void)arg;  // Cast to void to avoid unused variable warning
    printf("Alarm triggered!\n");
}

int main(void) {
    // Print initial message
    printf("Setting RTC alarm for 5 seconds...\n");

    // Define a timer
    xtimer_t timer;

    // Set a 5-second timer (5,000,000 microseconds)
    xtimer_set_absolute(&timer, xtimer_now() + (5 * 1000000));

    // Set the callback function for when the timer expires
    timer.callback = rtc_alarm_callback;
    timer.arg = NULL;

    // Start the timer
    xtimer_set(&timer, 5 * 1000000);

    // Enter sleeping mode to save power until the alarm triggers
    while (1) {
        xtimer_sleep(UINT32_MAX);  // Sleep, effectively forever, or until an interrupt
    }
    return 0;
}