#include <xtimer.h>
#include <stdio.h>
#include <time.h>
#include <stdint.h>
#include <inttypes.h>
#include <periph/rtc.h>

// Callback function for the RTC alarm
void rtc_alarm_cb(void *arg) {
    (void)arg; // Unused parameter
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time;
    rtc_t dev = RTC_DEV(0); // Corrected way to get RTC device descriptor
    
    // Initialize the RTC
    rtc_init(dev);
    
    // Set date to June 1, 2024, 00:00:00
    time.tm_year = 2024 - 1900; // Year since 1900
    time.tm_mon = 5; // Month, where 0 = January
    time.tm_mday = 1; // Day of the month
    time.tm_hour = 0; // Hour (24-hour clock)
    time.tm_min = 0; // Minutes
    time.tm_sec = 0; // Seconds

    // Set the current time
    rtc_set_time(&time);

    // Set the alarm to trigger 5 seconds later
    xtimer_sleep(5);
    rtc_get_time(&time); // Get the current time again
    time.tm_sec += 5;  // Set alarm 5 seconds from current time

    rtc_set_alarm(&time, rtc_alarm_cb, NULL);

    // Enter an infinite loop to keep the program running
    while (1) {
        xtimer_sleep(1); // Sleep to prevent busy-waiting
    }

    return 0;
}