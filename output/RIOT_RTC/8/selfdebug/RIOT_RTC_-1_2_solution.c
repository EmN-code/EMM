#include <stdio.h>
#include <stdint.h>
#include <periph/rtc.h>  // Include RTC peripheral header

void rtc_alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time;
    time.tm_year = 2024 - 1900; // Year since 1900
    time.tm_mon = 6 - 1; // Month, where 0 = January
    time.tm_mday = 1; // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    if (rtc_set_time(&time) != 0) {
        printf("Failed to set RTC time\n");
        return -1;
    }

    // Set alarm to trigger 5 seconds after the current time
    struct tm alarm_time = time;
    alarm_time.tm_sec += 5;

    rtc_alarm_t alarm = {
        .time = alarm_time,
        .cb = rtc_alarm_callback,
        .arg = NULL
    };

    if (rtc_set_alarm(&alarm) != 0) {
        printf("Failed to set RTC alarm\n");
        return -1;
    }

    printf("RTC alarm set for 5 seconds\n");

    // Keep the program running to wait for the callback
    while (1) {
        // Typically sleep or wait for an event here
    }

    return 0;
}