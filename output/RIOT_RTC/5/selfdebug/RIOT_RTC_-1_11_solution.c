#include <stdio.h>
#include <time.h>
#include <thread.h>
#include <xtimer.h>
#include <periph/rtc.h>

void alarm_callback(void *arg) {
    (void)arg;  // To avoid unused parameter warning
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time;
    time.tm_year = 2024 - 1900; // tm_year is year since 1900
    time.tm_mon = 5; // June (months since January 0-11)
    time.tm_mday = 1; // 1st
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    if (rtc_set_time(&time) != 0) {
        printf("Failed to set RTC time\n");
        return 1;
    }
    printf("RTC time set to 2024-06-01 00:00:00\n");

    // Get current time
    struct tm current_time;
    rtc_get_time(&current_time);

    // Set alarm time to 5 seconds later
    struct tm alarm_time = current_time;
    alarm_time.tm_sec += 5;
    mktime(&alarm_time); // Normalize the time structure after increment

    // Set the alarm
    if (rtc_set_alarm(&alarm_time, &alarm_callback, NULL) != 0) {
        printf("Failed to set RTC alarm\n");
        return 1;
    }
    printf("Alarm set to trigger in 5 seconds\n");

    while (1) {
        xtimer_sleep(10); // Keep main thread alive
    }
    return 0;
}