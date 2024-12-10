// Include necessary headers
#include <stdio.h>
#include "rtc.h"
#include "xtimer.h"

// Function to be called when the alarm is triggered
void alarm_callback(void *arg) {
    (void)arg; // Unused parameter
    puts("Alarm triggered!");
}

// Main entry point
int main(void) {
    // Initialize the RTC device
    rtc_init();

    // Define the desired date: June 1, 2024
    struct tm date;
    date.tm_year = 2024 - 1900;  // Year since 1900
    date.tm_mon = 5;             // Month (0-based index; 5 for June)
    date.tm_mday = 1;            // Day of the month
    date.tm_hour = 0;            // Hours (midnight)
    date.tm_min = 0;             // Minutes
    date.tm_sec = 0;             // Seconds

    // Set the RTC date
    rtc_set_time(&date);

    // Set an alarm to trigger after 5 seconds
    struct tm alarm_time = date;
    alarm_time.tm_sec += 5;

    // Set the alarm with the callback
    rtc_set_alarm(&alarm_time, alarm_callback, NULL);

    // Enter a low-power sleep mode (simulate sleep)
    xtimer_sleep(6);

    return 0;
}