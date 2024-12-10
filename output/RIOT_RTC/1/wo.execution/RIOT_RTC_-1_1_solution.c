#include <stdio.h>
#include <periph/rtc.h>
#include <xtimer.h>

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    // Define target date: June 1, 2024 00:00:00
    struct tm time;
    time.tm_year = 2024 - 1900;
    time.tm_mon = 5;
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    // Initialize RTC with the target time
    if (rtc_init() < 0) {
        puts("RTC initialization failed");
        return 1;
    }

    if (rtc_set_time(&time) < 0) {
        puts("RTC set time failed");
        return 1;
    }

    // Set an alarm to trigger after 5 seconds
    time_t now;
    rtc_get_time(&now);
    struct tm *now_tm = gmtime(&now);
    now_tm->tm_sec += 5;  // add 5 seconds
    mktime(now_tm);  // normalize time value

    if (rtc_set_alarm(now_tm, alarm_callback, NULL) < 0) {
        puts("RTC set alarm failed");
        return 1;
    }

    puts("Alarm set for 5 seconds from now.");

    // Sleep to wait for the alarm
    while (1) {
        xtimer_sleep(10);
    }

    return 0;
}