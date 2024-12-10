#include <stdio.h>
#include <rtc.h>
#include <xtimer.h>

void rtc_alarm_callback(void *arg) {
    (void)arg;  // Avoid unused parameter warning
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm date_time;
    date_time.tm_year = 2024 - 1900; // Year since 1900
    date_time.tm_mon = 5;  // Month (0-11, where 0 = January)
    date_time.tm_mday = 1; // Day of the month
    date_time.tm_hour = 0;
    date_time.tm_min = 0;
    date_time.tm_sec = 0;

    // Initialize the RTC
    while (rtc_init() != 0) {
        printf("RTC initialization failed, retrying...\n");
        xtimer_sleep(1);
    }

    while (rtc_set_time(&date_time) != 0) {
        printf("Setting RTC time failed, retrying...\n");
        xtimer_sleep(1);
    }

    // Set an alarm for 5 seconds in the future
    struct tm alarm_time = date_time;
    alarm_time.tm_sec += 5;
    mktime(&alarm_time); // Normalize the time

    while (rtc_set_alarm(&alarm_time, rtc_alarm_callback, NULL) != 0) {
        printf("Setting RTC alarm failed, retrying...\n");
        xtimer_sleep(1);
    }

    printf("RTC time set to %04d-%02d-%02d %02d:%02d:%02d\n", 
            date_time.tm_year + 1900, date_time.tm_mon + 1, date_time.tm_mday, 
            date_time.tm_hour, date_time.tm_min, date_time.tm_sec);
    printf("Alarm set for 5 seconds later\n");

    // Keep the program running to wait for the alarm
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}