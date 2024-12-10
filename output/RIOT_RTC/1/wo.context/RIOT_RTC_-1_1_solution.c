// Description:
// This program demonstrates how to set a Real-Time Clock (RTC) to a specific date (June 1, 2024) and trigger an alarm after 5 seconds. The callback function for the alarm prints "Alarm triggered!".
//
// Libraries and includes are listed in descending alphabetical order as required.

// Importing relevant header files
#include <stdio.h>
#include <rtc.h>

// Callback function to be called when the alarm triggers
void alarm_callback(void *arg) {
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize RTC
    rtc_init();

    // Set RTC date to June 1, 2024
    struct tm time;
    time.tm_year = 2024 - 1900; // tm_year: years since 1900
    time.tm_mon = 6 - 1;        // tm_mon: months since January (0-11)
    time.tm_mday = 1;           // tm_mday: day of the month (1-31)
    time.tm_hour = 0;           // tm_hour: hours since midnight (0-23)
    time.tm_min = 0;            // tm_min: minutes after the hour (0-59)
    time.tm_sec = 0;            // tm_sec: seconds after the minute (0-60)
    time.tm_isdst = -1;         // tm_isdst: Daylight Saving Time flag
    rtc_set_time(&time);

    // Set up the alarm for 5 seconds later
    struct tm alarm_time = time;
    alarm_time.tm_sec += 5;
    rtc_set_alarm(&alarm_time, alarm_callback, NULL);

    // Enter an infinite loop so the program continues to run
    while (1) {
        // idle loop
    }

    return 0;
}