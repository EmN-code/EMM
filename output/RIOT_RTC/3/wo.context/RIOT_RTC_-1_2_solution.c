
// Description: This example code sets the RTC time to June 1, 2024, and triggers an alarm 5 seconds later, printing "Alarm triggered!" when the alarm goes off.

#include <time.h>
#include <stdio.h>
#include <xtimer.h>  // For timing operations

// This function will be called when the alarm is triggered
timer_cb_t alarm_callback(void *arg) {
    (void)arg;  // Unused parameter
    printf("Alarm triggered!\n");
    return;  // Return immediately
}

int main(void) {
    // Set RTC time to June 1, 2024, 00:00:00
    struct tm timeinfo;
    timeinfo.tm_year = 2024 - 1900; // tm_year is years since 1900
    timeinfo.tm_mon = 6 - 1;        // tm_mon is 0-based (0 = January)
    timeinfo.tm_mday = 1;
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;
    timeinfo.tm_isdst = -1; // Not considering daylight saving time
    
    if (rtc_set_time(&timeinfo) != 0) {
        printf("Failed to set RTC time\n");
        return 1;  // Exit if unable to set time
    }
    printf("RTC time set to June 1, 2024\n");

    // Set alarm to trigger after 5 seconds
    xtimer_usleep(5000000); // Sleep for 5 seconds

    // Trigger the alarm
    alarm_callback(NULL);

    return 0;
}
