#include <stdio.h>
#include <time.h>
#include <xtimer.h>
#include <periph/rtc.h>

static void rtc_alarm_handler(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time;
    rtc_alarm_t alarm;

    // Set RTC time to June 1, 2024
    time.tm_year = 2024 - 1900;  // Year since 1900
    time.tm_mon = 5;             // Month (0-11)
    time.tm_mday = 1;            // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    if (rtc_set_time(&time) != 0) {
        printf("Failed to set RTC time\n");
        return 1;
    }

    // Set an alarm for 5 seconds later
    if (rtc_get_time(&time) != 0) {
        printf("Failed to get current RTC time\n");
        return 1;
    }

    xtimer_sleep(5);

    alarm.time.tm_year = time.tm_year;
    alarm.time.tm_mon = time.tm_mon;
    alarm.time.tm_mday = time.tm_mday;
    alarm.time.tm_hour = time.tm_hour;
    alarm.time.tm_min = time.tm_min;
    alarm.time.tm_sec = time.tm_sec + 5;  // Trigger 5 seconds later

    alarm.callback = rtc_alarm_handler;
    alarm.arg = NULL;

    // Set the alarm and check for errors
    while (rtc_set_alarm(&alarm) != 0) {
        printf("Failed to set alarm. Retrying in 1 second...\n");
        xtimer_sleep(1);
    }

    printf("Alarm set. Waiting for 5 seconds...\n");

    // Wait for the alarm
    while(1) {
        // The RTC driver will call rtc_alarm_handler after the alarm is triggered
    }

    return 0;
}