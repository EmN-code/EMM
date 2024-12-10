#include <stdio.h>
#include <ztimer_sleep.h>
#include <ztimer.h>
#include <timex.h>
#include <periph/rtc.h>

void alarm_callback(void *arg) {
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize the RTC module
    if (rtc_init() != 0) {
        printf("Failed to initialize RTC\n");
        return -1;
    }

    // Set initial RTC time to June 1, 2024, 00:00:00
    struct tm time = {
        .tm_year = 2024 - 1900,  // tm_year is years since 1900
        .tm_mon = 5,             // tm_mon is 0-based, thus 5 for June
        .tm_mday = 1,
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };

    if (rtc_set_time(&time) != 0) {
        printf("Failed to set RTC time\n");
        return -1;
    }

    // Create an alarm to trigger 5 seconds from the current time
    rtc_alarm_t alarm;
    rtc_get_time(&time);
    alarm.time = time;
    alarm.time.tm_sec += 5;  // Add 5 seconds
    alarm.callback = alarm_callback;
    alarm.arg = NULL;

    if (rtc_set_alarm(&alarm) != 0) {
        printf("Failed to set RTC alarm\n");
        return -1;
    }

    printf("RTC alarm set for 5 seconds from now.\n");

    return 0;
}
