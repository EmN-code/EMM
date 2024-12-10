// Importing necessary headers
#include "xtimer.h"
#include "ztimer.h"
#include "stdio.h"
#include "periph/rtc.h"

// Callback function for the RTC alarm
void rtc_alarm_callback(void *arg) {
    (void)arg; // Unused parameter
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize the RTC
    if (rtc_init() != 0) {
        printf("Error initializing RTC\n");
        return -1;
    }

    // Set date to June 1, 2024
    struct tm time;
    time.tm_year = 2024 - 1900; // Year since 1900
    time.tm_mon = 5;           // Month 0-11
    time.tm_mday = 1;          // Day of the month 1-31
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    if (rtc_set_time(&time) != 0) {
        printf("Failed to set RTC time\n");
        return -1;
    }

    // Set alarm to 5 seconds from the current time
    ztimer_sleep(ZTIMER_MSEC, 5000); // Wait 5 seconds before setting the alarm
    if (rtc_set_alarm(NULL, rtc_alarm_callback, NULL) != 0) {
        printf("Failed to set RTC alarm\n");
        return -1;
    }

    // Enter a low-power state until the alarm triggers
    ztimer_sleep(ZTIMER_MSEC, 6000); // Wait 6 seconds to ensure the alarm triggers

    return 0;
}