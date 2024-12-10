#include <xtimer.h>
#include <stdio.h>
#include <rtc.h>
#include <time.h>

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm timeinfo;
    rtc_alarm_t alarm;

    // Set the date to June 1, 2024
    timeinfo.tm_year = 2024 - 1900; // years since 1900
    timeinfo.tm_mon = 6 - 1;        // months since January
    timeinfo.tm_mday = 1;           // day of the month
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;
    timeinfo.tm_isdst = -1;         // no daylight saving time

    // Initialize RTC with this date
    if (rtc_set_time(&timeinfo) != 0) {
        printf("RTC initialization failed!\n");
        return 1;
    }

    // Set the alarm to trigger 5 seconds from the current time
    alarm.time = rtc_get_time() + 5;
    alarm.callback = alarm_callback;
    alarm.arg = NULL;

    if (rtc_set_alarm(&alarm) != 0) {
        printf("RTC alarm setup failed!\n");
        return 1;
    }

    printf("Waiting for the alarm to trigger...\n");

    // Wait for a long time to ensure that the alarm has time to trigger
    while (1) {
        xtimer_sleep(60);
    }

    return 0;
}