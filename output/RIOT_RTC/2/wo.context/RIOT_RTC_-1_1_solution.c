#include <stdio.h>
#include <time.h>
#include <rtc.h>

void alarm_callback(void *arg) {
    printf("Alarm triggered!\n");
}

int main(void) {
    // Set the targeted time to June 1, 2024
    struct tm timeinfo;
    timeinfo.tm_year = 2024 - 1900; // tm_year is years since 1900
    timeinfo.tm_mon = 6 - 1; // zero-based
    timeinfo.tm_mday = 1;
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;
    timeinfo.tm_isdst = -1; // Not in daylight saving time

    // Initialize RTC
    rtc_init();

    // Set RTC time
    rtc_set_time(&timeinfo);

    // Set an alarm to trigger after 5 seconds
    struct tm alarm_time;
    alarm_time = timeinfo;
    alarm_time.tm_sec += 5;
    mktime(&alarm_time); // Normalize the time structure

    // Set RTC alarm
    rtc_set_alarm(&alarm_time, alarm_callback, NULL);

    // Wait for the alarm to trigger
    while (1) {
        // Sleep or busy-wait
    }

    return 0;
}