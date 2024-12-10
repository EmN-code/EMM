#include <ztimer.h>
#include <stdio.h>
#include <time.h>

// Function called when the alarm triggers
void alarm_callback(void *arg) {
    puts("Alarm triggered!");
}

int main(void) {
    struct tm time;
    ztimer_now(ZTIMER_MSEC);

    // Set the date and time to June 1, 2024, 00:00:00
    time.tm_year = 2024 - 1900;  // Year since 1900
    time.tm_mon = 5;             // Months since January
    time.tm_mday = 1;            // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    // Function to set RTC time using ztimer
    rtc_set_time((mktime(&time) * 1000ULL) + ztimer_now(ZTIMER_MSEC));

    // Schedule an alarm after 5 seconds
    ztimer_set(ZTIMER_MSEC, &alarm_callback, NULL, 5000);  // 5 seconds in future

    // Letting the main thread sleep, so callbacks can be processed
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);  // Sleep 1 second intervals
    }
    return 0;
}