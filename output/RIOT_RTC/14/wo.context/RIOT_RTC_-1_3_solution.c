#include <stdio.h>
#include <xtimer.h>
#include <periph/rtc.h>

#define RTC_YEAR   2024
#define RTC_MONTH  6  // June
#define RTC_DAY    1

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    // Setting RTC initial date and time
    struct tm init_time = {
        .tm_year = RTC_YEAR - 1900,  // tm_year is year since 1900
        .tm_mon  = RTC_MONTH - 1,    // tm_mon is 0-based
        .tm_mday = RTC_DAY,
        .tm_hour = 0,
        .tm_min  = 0,
        .tm_sec  = 0
    };

    if (rtc_set_time(&init_time) != 0) {
        printf("Failed to set RTC time\n");
        return 1;
    }

    // Setting up the alarm
    struct tm alarm_time;
    rtc_get_time(&alarm_time);

    // Increment seconds by 5
    alarm_time.tm_sec += 5;
    mktime(&alarm_time);  // Normalize the time structure

    rtc_alarm_cb_t alarm;
    alarm.callback = alarm_callback;
    alarm.time = &alarm_time;
    alarm.arg = NULL;

    if (rtc_set_alarm(&alarm, NULL) != 0) {
        printf("Failed to set RTC alarm\n");
        return 1;
    }

    printf("RTC alarm set for 5 seconds later. Waiting for alarm to trigger...\n");

    while (1) {
        // Loop infinitely to keep the main program running
        xtimer_sleep(1);
    }

    return 0;
}