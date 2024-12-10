#include <stdio.h>
#include <xtimer.h>
#include <periph/rtc.h>

void rtc_alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time = {
        .tm_year = 2024 - 1900, // Year since 1900
        .tm_mon = 6 - 1,        // Months since January
        .tm_mday = 1,           // Day of the month
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };

    rtc_set_time(&time);

    rtc_alarm_t alarm;
    alarm.time = rtc_get_time() + 5;  // Set alarm 5 seconds from now
    alarm.callback = rtc_alarm_callback;
    alarm.arg = NULL;

    if (rtc_set_alarm(&alarm) != 0) {
        printf("Error: Could not set the RTC alarm\n");
        return -1;
    }

    while(1) {
        xtimer_sleep(1);
    }

    return 0;
}