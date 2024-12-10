#include <stdio.h>
#include <periph_cpu.h>

void alarm_callback(void *arg) {
    puts("Alarm triggered!");
}

int main(void) {
    struct tm time_info;

    // Set the date to June 1, 2024
    time_info.tm_year = 2024 - 1900;  // years since 1900
    time_info.tm_mon = 5;            // months since January (0-11)
    time_info.tm_mday = 1;           // day of the month (1-31)
    time_info.tm_hour = 0;
    time_info.tm_min = 0;
    time_info.tm_sec = 0;

    rtc_set_time(&time_info);

    // Set the alarm to trigger after 5 seconds
    int alarm_time = rtc_get_time() + 5;
    rtc_set_alarm(alarm_time, alarm_callback, NULL);

    while (1) {
        // Loop indefinitely
    }
    return 0;
}