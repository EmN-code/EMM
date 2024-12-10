#include <xtimer.h>
#include <stdio.h>
#include <time.h>

#define ALARM_DELAY  5  // 5 seconds delay

static void rtc_alarm_cb(void *arg) {
    (void)arg;     // Avoid unused parameter warning
    puts("Alarm triggered!");
}

int main(void) {
    struct tm set_time;
    // Set the date to June 1, 2024
    set_time.tm_year = 2024 - 1900;   // tm structure year starts from 1900
    set_time.tm_mon = 6 - 1;          // tm structure month starts from 0
    set_time.tm_mday = 1;
    set_time.tm_hour = 0;
    set_time.tm_min = 0;
    set_time.tm_sec = 0;
    set_time.tm_isdst = -1;           // No daylight saving information

    // Initialize the RTC with the set time
    rtc_set_time(&set_time);

    // Set the alarm
    rtc_set_alarm(&set_time, rtc_alarm_cb, NULL);

    // Wait for the alarm to trigger after ALARM_DELAY seconds
    xtimer_sleep(ALARM_DELAY);
    printf("Waiting for alarm to trigger...\n");

    return 0;
}