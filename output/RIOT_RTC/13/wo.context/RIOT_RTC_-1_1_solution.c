#include "rtc.h"
#include "msg.h"
#include "kernel.h"

void rtc_alarm_callback(void *arg) {
    (void)arg;  // Unused parameter
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize a RTC time structure to set date and time
    struct tm time;
    
    // Set date to June 1, 2024
    time.tm_year = 2024 - 1900; // Years since 1900
    time.tm_mon = 6 - 1;        // Months since January
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    // Set the RTC to the specified time
    if (rtc_set_time(&time) != 0) {
        printf("Failed to set RTC time!\n");
        return -1;
    }

    // Set an alarm for 5 seconds from now
    struct tm alarm_time;
    time.tm_sec += 5;  // Add 5 seconds to the current time

    if (rtc_set_alarm(&alarm_time, rtc_alarm_callback, NULL) != 0) {
        printf("Failed to set RTC alarm!\n");
        return -1;
    }

    // Infinite loop to wait for the alarm
    while (1) {
        // Do nothing, just wait for the alarm to trigger
    }

    return 0;
}