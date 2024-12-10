#include <stdio.h>
#include <time.h>

void rtc_alarm_callback(void *arg) {
    printf("Alarm triggered!\n");
}

int main() {
    // Define and set the time for June 1, 2024
    struct tm tm;
    tm.tm_year = 2024 - 1900; // tm_year counts years since 1900
    tm.tm_mon = 6 - 1; // tm_mon is 0-indexed
    tm.tm_mday = 1;
    tm.tm_hour = 0;
    tm.tm_min = 0;
    tm.tm_sec = 0;
    time_t time_now = mktime(&tm);
    
    // Set RTC time (this is a hypothetical function, depends on RTC driver)
    rtc_set_time(&time_now);

    // Set a 5 second alarm
    struct tm alarm_time = *localtime(&time_now);
    alarm_time.tm_sec += 5;
    mktime(&alarm_time);  // Normalize

    rtc_set_alarm(&alarm_time, rtc_alarm_callback, NULL);

    // Main loop
    while (1) {
        // Perform other tasks
    }

    return 0;
}