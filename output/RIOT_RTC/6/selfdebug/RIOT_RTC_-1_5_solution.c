#include <stdio.h>
#include <time.h>
#include <xtimer.h>

void alarm_callback(void *arg) {
    (void)arg;  // unused
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time_info;
    rtc_alarm_cb_t alarm_cb = alarm_callback;
    time_t now;

    // Set current time to June 1, 2024, 00:00:00
    time_info.tm_year = 2024 - 1900;
    time_info.tm_mon = 6 - 1;
    time_info.tm_mday = 1;
    time_info.tm_hour = 0;
    time_info.tm_min = 0;
    time_info.tm_sec = 0;
    time_info.tm_isdst = -1; // Daylight Saving Time flag

    // Convert to time_t to set the system time
    now = mktime(&time_info);
    set_time(now);

    // Set up the alarm for 5 seconds later
    struct tm alarm_time = time_info;
    alarm_time.tm_sec += 5;
    mktime(&alarm_time);  // Adjust the alarm time structure

    // Set the alarm
    rtc_set_alarm(&alarm_time, alarm_cb, NULL);

    // Loop to keep the program running
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}