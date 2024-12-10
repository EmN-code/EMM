#include "ztimer.h"
#include "timex.h"
#include "periph/rtc.h"

void rtc_alarm_callback(void *arg) {
    (void)arg;  // Unused parameter
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize RTC
    if (rtc_init() != 0) {
        printf("RTC initialization failed!\n");
        return -1;
    }

    // Set the RTC time to June 1, 2024, 00:00:00
    struct tm time;
    time.tm_year = 2024 - 1900;  // tm_year is years since 1900
    time.tm_mon = 6 - 1;         // tm_mon is months since January
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    if (rtc_set_time(&time) != 0) {
        printf("RTC set time failed!\n");
        return -1;
    }

    // Configure the alarm for 5 seconds later
    time_t now = rtc_get_time();
    if (now == (time_t)-1) {
        printf("RTC get time failed!\n");
        return -1;
    }
    rtc_alarm_t alarm;
    alarm.time = now + 5;
    alarm.callback = rtc_alarm_callback;
    alarm.arg = NULL;

    if (rtc_set_alarm(&alarm) != 0) {
        printf("RTC set alarm failed!\n");
        return -1;
    }

    // Block the program to wait for the alarm (typically would sleep or continue other tasks)
    while (1) {
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}