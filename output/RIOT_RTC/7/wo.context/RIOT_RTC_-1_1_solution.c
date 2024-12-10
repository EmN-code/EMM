#include <time.h>
#include <stdio.h>

// Function to be called when alarm is triggered
void rtc_alarm_callback(void *arg) {
    printf("Alarm triggered!\n");
}

int main() {
    struct tm time_info;
    
    // Set the RTC time to June 1, 2024
    time_info.tm_year = 2024 - 1900; // Year since 1900
    time_info.tm_mon = 6 - 1; // Month, where 0 = January
    time_info.tm_mday = 1; // Day of the month
    time_info.tm_hour = 0;
    time_info.tm_min = 0;
    time_info.tm_sec = 0;

    // Initialize the RTC with given time
    rtc_init();
    rtc_set_time(&time_info);

    // Set an alarm for 5 seconds later
    struct tm alarm_time = time_info;
    alarm_time.tm_sec += 5; // Add 5 seconds
    rtc_set_alarm(&alarm_time, rtc_alarm_callback, NULL);

    // Enter loop waiting for the alarm to trigger
    while (1) {
        // Sleep or do other tasks
    }

    return 0;
}