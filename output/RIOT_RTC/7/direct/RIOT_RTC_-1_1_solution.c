#include <xtimer.h>
#include <stdio.h>
#include <time.h>
#include <timex.h>
#include <periph/rtc.h>

#define ALARM_DELAY (5U)  // 5 seconds delay for the alarm

static void rtc_alarm_cb(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time;
    rtc_alarm_t alarm;

    // Set the date to June 1, 2024
    time.tm_year = 2024 - 1900;
    time.tm_mon = 6 - 1;  // Months are 0-11 in tm struct
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    if (rtc_set_time(&time) != 0) {
        printf("Error: Could not set RTC time\n");
        return 1;
    }

    // Schedule an alarm
    alarm.tm = time;
    alarm.tm.tm_sec += ALARM_DELAY; // Set alarm 5 seconds from now

    // Correct for time overflow if necessary
    mktime(&alarm.tm);

    // Set the alarm with the callback function
    if (rtc_set_alarm(&alarm, rtc_alarm_cb, NULL) != 0) {
        printf("Error: Could not set RTC alarm\n");
        return 1;
    }

    printf("Alarm set for 5 seconds from current time\n");

    while (1) {
        xtimer_sleep(1); // Sleep to allow RTC to trigger alarms
    }

    return 0;
}
