#include <stdio.h>
#include <xtimer.h>
#include <periph/rtc.h>

#define ALARM_DURATION (5U) // 5 seconds

static void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm timeinfo;

    // Set RTC time to June 1, 2024, 00:00:00
    timeinfo.tm_year = 2024 - 1900; // Year since 1900
    timeinfo.tm_mon = 5;            // Months since January [0-11]
    timeinfo.tm_mday = 1;           // Day of the month [1-31]
    timeinfo.tm_hour = 0;           // Hours since midnight [0-23]
    timeinfo.tm_min = 0;            // Minutes after the hour [0-59]
    timeinfo.tm_sec = 0;            // Seconds after the minute [0-60]

    if (rtc_set_time(&timeinfo) != 0) {
        printf("Error: Unable to set RTC time\n");
        return -1;
    }

    // Set alarm time
    struct tm alarm_time = timeinfo;
    time_t alarm_epoch = mktime(&timeinfo) + ALARM_DURATION;
    localtime_r(&alarm_epoch, &alarm_time);

    if (rtc_set_alarm(&alarm_time, alarm_callback, NULL) != 0) {
        printf("Error: Unable to set RTC alarm\n");
        return -1;
    }

    printf("RTC time set to June 1, 2024, and alarm will trigger in 5 seconds.\n");

    // Enter an idle loop, waiting for the alarm
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}