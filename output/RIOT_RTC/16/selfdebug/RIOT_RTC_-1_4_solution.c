#include <stdio.h>
#include <stdlib.h>
#include "xtimer.h"
#include "periph/rtc.h"

void set_rtc_alarm(void); // Forward declaration

void rtc_alarm_callback(void *arg) {
    (void)arg; // Unused parameter
    printf("Alarm triggered!\n");
}

int main(void) {
    printf("Setting RTC date to June 1, 2024 and scheduling an alarm.\n");

    rtc_time_t time;
    time.tm_year = 2024; // Year
    time.tm_mon  = 6;    // Month
    time.tm_mday = 1;    // Day
    time.tm_hour = 0;
    time.tm_min  = 0;
    time.tm_sec  = 0;

    if (rtc_set_time(&time) < 0) {
        printf("Error: Unable to set the RTC time.\n");
        return 1;
    }

    set_rtc_alarm();

    while (1) {
        xtimer_sleep(1); // Keep the main thread alive
    }

    return 0;
}

void set_rtc_alarm(void) {
    rtc_time_t alarm_time;
    rtc_get_time(&alarm_time);

    alarm_time.tm_sec += 5;
    mktime(&alarm_time); // Normalize the time structure

    if (rtc_set_alarm(&alarm_time, rtc_alarm_callback, NULL) < 0) {
        printf("Error: Unable to set the RTC alarm.\n");
    } else {
        printf("Alarm set to trigger in 5 seconds.\n");
    }
}