#include <stdio.h>
#include <periph/rtc.h>
#include <xtimer.h>

void rtc_alarm_cb(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    // Set up RTC
    if (rtc_init() != 0) {
        printf("Failed to initialize RTC\n");
        return 1;
    }

    // Set date to June 1, 2024, 00:00:00
    struct tm time = {
        .tm_year = 2024 - 1900,  // years since 1900
        .tm_mon = 5,             // months since January [0-11]
        .tm_mday = 1,
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };

    if (rtc_set_time(&time) != 0) {
        printf("Failed to set RTC time\n");
        return 1;
    }

    // Create an alarm 5 seconds from now
    struct tm alarm_time;
    if (rtc_get_time(&alarm_time) != 0) {
        printf("Failed to get current RTC time\n");
        return 1;
    }

    alarm_time.tm_sec += 5;
    mktime(&alarm_time);  // Normalize time structure

    if (rtc_set_alarm(&alarm_time, rtc_alarm_cb, NULL) != 0) {
        printf("Failed to set alarm\n");
        return 1;
    }

    // Wait for alarm
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}