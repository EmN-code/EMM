#include <xtimer.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/bytes.h>
#include <stdlib.h>

rtctime_t current_time;

void rtc_alarm_callback(void *arg) {
    (void) arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    printf("RTC Alarm Setting Example\n");

    // Set the current date to June 1, 2024
    current_time.tm_year = 2024 - 1900; // Year since 1900
    current_time.tm_mon = 5;            // 0-based, so June is 5
    current_time.tm_mday = 1;           // Day of the month
    current_time.tm_hour = 0;
    current_time.tm_min = 0;
    current_time.tm_sec = 0;

    // Initialize RTC with current time
    rtc_set_time(&current_time);

    // Set an alarm to trigger in 5 seconds
    rtctime_t alarm_time = current_time;
    alarm_time.tm_sec += 5;

    rtc_set_alarm(&alarm_time, rtc_alarm_callback, NULL);

    // Main loop that waits for alarm
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}