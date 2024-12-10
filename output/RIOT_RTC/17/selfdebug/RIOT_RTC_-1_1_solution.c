#include <stdio.h>
#include <time.h>

// Function to be called when alarm is triggered
void alarm_callback(void) {
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize RTC time to June 1, 2024
    struct tm rtc_time = {
        .tm_year = 2024 - 1900, // Year since 1900
        .tm_mon = 6 - 1,        // Month 0-11
        .tm_mday = 1,           // Day of the month
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };

    // Set RTC time
    rtc_set_time(&rtc_time);

    // Set alarm for 5 seconds from now
    rtc_time.tm_sec += 5;
    rtc_set_alarm(&rtc_time, alarm_callback, NULL);

    while (1) {
        // Main loop can do other tasks
    }

    return 0;
}
