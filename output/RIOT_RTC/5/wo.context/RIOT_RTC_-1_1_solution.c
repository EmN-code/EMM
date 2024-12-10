#include <time.h>
#include <stdio.h>
#include <rtc.h>

void rtc_alarm_cb(void *arg) {
    (void)arg;  // Unused variable
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize RTC
    rtc_init();

    // Set RTC date and time to June 1, 2024, 00:00:00
    struct tm timeinfo;
    timeinfo.tm_year = 2024 - 1900;  // Year since 1900
    timeinfo.tm_mon = 5;             // 0-indexed month (0=Jan, ..., 5=Jun)
    timeinfo.tm_mday = 1;            // Day of the month
    timeinfo.tm_hour = 0;            // Hour
    timeinfo.tm_min = 0;             // Minute
    timeinfo.tm_sec = 0;             // Second
    timeinfo.tm_isdst = -1;          // No daylight saving time

    if (rtc_set_time(&timeinfo) != 0) {
        printf("Failed to set RTC time\n");
        return -1;
    }

    // Set alarm to trigger 5 seconds after the current RTC time
    struct tm alarm_time = timeinfo;
    alarm_time.tm_sec += 5;
    mktime(&alarm_time);  // Normalize the time structure

    if (rtc_set_alarm(&alarm_time, rtc_alarm_cb, NULL) != 0) {
        printf("Failed to set RTC alarm\n");
        return -1;
    }

    printf("RTC initialized and alarm set for 5 seconds.\n");

    // Keep the program running so that the alarm can trigger
    while (1) {
        // Simulate doing nothing, waiting for alarm
    }

    return 0;
}