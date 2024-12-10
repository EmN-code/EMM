#include <stdio.h>
#include <xtimer.h>
#include <periph/rtc.h>

// Callback function for the RTC alarm
void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time;
    struct tm alarm_time;

    // Set the RTC time to June 1, 2024, 00:00:00
    time.tm_year = 2024 - 1900; // Year since 1900
    time.tm_mon = 6 - 1;        // Months since January
    time.tm_mday = 1;           // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    if (rtc_set_time(&time) < 0) {
        printf("Error setting the RTC time\n");
        return 1;
    }
    printf("RTC time set to June 1, 2024, 00:00:00\n");

    // Set the alarm to trigger after 5 seconds
    xtimer_sleep(5);
    rtc_get_time(&alarm_time);
    alarm_time.tm_sec += 5; // Add 5 seconds to the current time

    if (rtc_set_alarm(&alarm_time, alarm_callback, NULL) < 0) {
        printf("Error setting the RTC alarm\n");
        return 1;
    }

    while (1) {
        // Main loop can perform other tasks
    }

    return 0;
}