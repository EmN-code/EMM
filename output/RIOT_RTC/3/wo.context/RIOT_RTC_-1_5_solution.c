#include <ztimer.h>
#include <stdio.h>
#include <time.h>
#include <rtc.h>

void alarm_cb(void *arg) {
    (void)arg;
    puts("Alarm triggered!");
}

int main(void) {
    // Initialize RTC
    if (rtc_init() != 0) {
        puts("Error initializing RTC");
        return 1;
    }

    // Set the RTC to June 1, 2024
    struct tm timeinfo = {
        .tm_year = 2024 - 1900,
        .tm_mon = 5,
        .tm_mday = 1,
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };
    if (rtc_set_time(&timeinfo) != 0) {
        puts("Error setting time");
        return 1;
    }

    // Prepare the alarm to trigger after 5 seconds
    rtc_alarm_t alarm;
    rtc_get_time(&alarm.time);
    ztimer_sleep(ZTIMER_MSEC, 5000);
    alarm.time.tm_sec += 5;
    mktime(&alarm.time);
    alarm.callback = alarm_cb;
    alarm.arg = NULL;

    // Set the RTC alarm
    if (rtc_set_alarm(&alarm) != 0) {
        puts("Error setting alarm");
        return 1;
    }

    puts("RTC alarm set. Waiting for alarm...");

    // Wait indefinitely for the alarm to trigger
    while (1) {
        ztimer_sleep(ZTIMER_USEC, 1000000);
    }

    return 0;
}