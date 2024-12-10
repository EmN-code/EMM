#include <stdio.h>  // For printf function
#include <time.h>   // For manipulating time and RTC settings
#include <rtc.h>    // For real-time clock functionalities

void alarm_callback(void *arg) {
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time_info;
    time_info.tm_year = 2024 - 1900;  // Years since 1900
    time_info.tm_mon = 6 - 1;         // Months since January
    time_info.tm_mday = 1;            // Day of the month
    time_info.tm_hour = 0;
    time_info.tm_min = 0;
    time_info.tm_sec = 0;

    // Initialize RTC with the current time
    if (rtc_init() != 0) {
        printf("RTC initialization failed!\n");
        return -1;
    }

    // Set RTC time to June 1, 2024
    if (rtc_set_time(&time_info) != 0) {
        printf("Failed to set RTC time!\n");
        return -1;
    }

    // Set an alarm to trigger 5 seconds from the current time
    time_info.tm_sec += 5;

    if (rtc_set_alarm(&time_info, alarm_callback, NULL) != 0) {
        printf("Failed to set RTC alarm!\n");
        return -1;
    }

    printf("RTC Alarm is set for June 1, 2024, 00:00:05!\n");

    while (1) {
        // Keep the program running to wait for the alarm
    }

    return 0;
}