#include <stdio.h>
#include <time.h>
#include <sys/types.h>

void rtc_alarm_cb(void *arg) {
    (void)arg;
    puts("Alarm triggered!");
}

int main(void) {
    struct tm time_info;
    time_t now;

    // Set the date to June 1, 2024
    time_info.tm_year = 2024 - 1900; // Year since 1900
    time_info.tm_mon = 6 - 1;        // Month, where 0 = jan
    time_info.tm_mday = 1;           // Day of the month
    time_info.tm_hour = 0;
    time_info.tm_min = 0;
    time_info.tm_sec = 0;
    time_info.tm_isdst = -1;         // Is DST on? 1 = yes, 0 = no, -1 = unknown

    // Convert to time_t and set the RTC
    now = mktime(&time_info);
    if (now == -1) {
        puts("Failed to set RTC time");
        return 1;
    }

    // Assuming rtc_set_time is a function to set RTC time, replace with actual function
    // rtc_set_time(now);

    // Set an alarm 5 seconds from "now"
    time_t alarm_time = now + 5;

    // Assuming rtc_set_alarm is a function to set RTC alarm, replace with actual function
    // rtc_set_alarm(alarm_time, rtc_alarm_cb, NULL);

    // Simulate waiting, typically in an embedded system this would not be done like this
    sleep(6);  // Just to simulate the passing time in this example

    return 0;
}