#include <stdio.h>
#include "xtimer.h"
#include "periph/rtc.h"

void alarm_callback(void *arg) {
    (void) arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    rtc_time_t time;
    rtc_alarm_t alarm;

    // Set the RTC date to June 1, 2024
    time.tm_year = 2024;
    time.tm_mon = 6;
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    rtc_set_time(&time);

    // Set an alarm 5 seconds in the future
    rtc_get_time(&time); // Get current time
    alarm.time = time;
    alarm.time.tm_sec += 5;

    // Adjust for any overflow in seconds (very simplistic wrap-around fix)
    if (alarm.time.tm_sec >= 60) {
        alarm.time.tm_sec -= 60;
        alarm.time.tm_min += 1;
    }

    alarm.callback = alarm_callback;
    alarm.arg = NULL;

    if (rtc_set_alarm(&alarm) != 0) {
        printf("Error setting the alarm\n");
        return 1;
    }

    printf("Alarm set for 5 seconds from now\n");

    while (1) {
        // Sleep/idle until the alarm triggers
        xtimer_sleep(1);
    }

    return 0;
}