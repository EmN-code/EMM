#include <stdio.h>
#include <time.h>  // Necessary for struct tm
#include "periph/rtc.h"  // Include the RTC peripheral for time and alarm management

// Callback function that gets called when the alarm is triggered
void alarm_callback(void *arg) {
    (void)arg; // Ignore unused parameter warning
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm timeinfo;

    // Set the desired date to June 1, 2024
    timeinfo.tm_year = 2024 - 1900;  // Year since 1900
    timeinfo.tm_mon = 5;             // Month, 0-11
    timeinfo.tm_mday = 1;            // Day of the month
    timeinfo.tm_hour = 0;            // Hour, 0-23
    timeinfo.tm_min = 0;             // Minute, 0-59
    timeinfo.tm_sec = 0;             // Second, 0-59

    // Initialize RTC with the specified time
    if (rtc_set_time(&timeinfo) < 0) {
        printf("Error setting RTC time\n");
        return 1;
    }
    printf("RTC time set to June 1, 2024\n");

    // Set an alarm for 5 seconds later
    timeinfo.tm_sec += 5;
    if (rtc_set_alarm(&timeinfo, alarm_callback, NULL) < 0) {
        printf("Error setting RTC alarm\n");
        return 1;
    }

    printf("Alarm set for 5 seconds from now\n");

    // Main loop to keep the program running
    while(1) {
        // Sleep to prevent busy-waiting (simplifies for example)
        sleep(1);
    }

    return 0;
}