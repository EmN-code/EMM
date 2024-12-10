#include <time.h>
#include <stdio.h>
#include <periph/rtc.h>

void rtc_alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize RTC
    rtc_init();

    // Set the current date to June 1, 2024
    struct tm current_time = {0};
    current_time.tm_year = 2024 - 1900; // tm_year is year since 1900
    current_time.tm_mon = 5; // June, 0-indexed (0 = January, ..., 5 = June)
    current_time.tm_mday = 1; // 1st day of the month
    current_time.tm_hour = 0;
    current_time.tm_min = 0;
    current_time.tm_sec = 0;
    time_t t = mktime(&current_time);
    rtc_set_time(t); // Set the RTC time

    // Set an alarm for 5 seconds in the future
    rtc_alarm_t alarm;
    alarm.time = rtc_get_time() + 5;
    alarm.callback = rtc_alarm_callback;
    alarm.arg = NULL;
    rtc_set_alarm(&alarm);

    while (1) {
        // Main loop can perform other tasks
    }

    return 0;
}