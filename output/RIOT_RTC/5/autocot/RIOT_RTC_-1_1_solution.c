#include <time.h>
#include <stdio.h>
#include <rtc.h>

// Function declaration for the alarm callback
void alarm_callback(void *arg);

int main(void) {
    // Initialize the RTC 
    struct tm timeinfo;
    timeinfo.tm_year = 2024 - 1900;  // Year since 1900
    timeinfo.tm_mon = 6 - 1;         // Month, 0 - Jan
    timeinfo.tm_mday = 1;            // Day of the month
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;
    rtc_init(); // Initialize RTC
    rtc_set_time(&timeinfo);

    // Set an alarm for 5 seconds later
    struct tm alarm_time;
    alarm_time.tm_sec = 5;    // Set 5 seconds alarm
    rtc_set_alarm(&alarm_time, alarm_callback, NULL);

    // Main loop does nothing, system sleep or other tasks
    while (1) {
        // Typically you'd have more application logic here
        // but for now just sleep to save power
    }

    return 0;
}

// Alarm callback
void alarm_callback(void *arg) {
    printf("Alarm triggered!\n");
}