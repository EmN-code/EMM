#include <stdio.h>
#include <time.h>
#include "rtc.h"
#include "thread.h"

void alarm_callback(void *arg) {
    puts("Alarm triggered!");
}

int main(void) {
    struct tm timeinfo;
    timeinfo.tm_year = 2024 - 1900; // Year since 1900
    timeinfo.tm_mon = 6 - 1;         // Month (0-11)
    timeinfo.tm_mday = 1;           // Day of the month (1-31)
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;
    rtc_init();
    rtc_set_time(&timeinfo);

    // Calculate the time for the alarm: current time + 5 seconds
    struct tm alarm_time = timeinfo;
    alarm_time.tm_sec += 5;
    mktime(&alarm_time); // Normalize the time structure

    // Set the RTC alarm
    rtc_set_alarm(&alarm_time, alarm_callback, NULL);

    // Keeping the program alive to wait for the alarm
    while (1) {
        thread_sleep();
    }

    return 0;
}