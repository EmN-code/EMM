#include <xtimer.h>
#include <stdlib.h>
#include <stdio.h>
#include <periph/rtc.h>

static void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time = {
        .tm_year = 2024 - 1900, // Year since 1900
        .tm_mon = 6 - 1,        // Months since January [0-11]
        .tm_mday = 1,           // Day of the month [1-31]
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };

    while (rtc_set_time(&time) < 0) {
        printf("Failed to set RTC time. Retrying in 1 second...\n");
        xtimer_sleep(1);
    }
    printf("Time set to: %04d-%02d-%02d %02d:%02d:%02d\n", 
            time.tm_year + 1900, time.tm_mon + 1, time.tm_mday, time.tm_hour, time.tm_min, time.tm_sec);

    struct tm alarm_time;
    rtc_get_alarm(&alarm_time);
    printf("Current alarm set for: %04d-%02d-%02d %02d:%02d:%02d\n", 
            alarm_time.tm_year + 1900, alarm_time.tm_mon + 1, alarm_time.tm_mday, alarm_time.tm_hour, alarm_time.tm_min, alarm_time.tm_sec);

    while (rtc_set_alarm(&alarm_time, alarm_callback, NULL) < 0) {
        printf("Failed to set RTC alarm. Retrying in 1 second...\n");
        xtimer_sleep(1);
    }
    printf("Alarm set to trigger after 5 seconds.\n");

    return 0;
}