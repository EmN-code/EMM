#include <stdio.h>
#include <time.h>
#include "thread.h"
#include "rtc.h"

void rtc_alarm_callback(void *arg) {
    (void)arg; // Unused parameter
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize the RTC
    rtc_init();

    // Set the date to June 1, 2024
    struct tm timeinfo = {
        .tm_year = 2024 - 1900, // Year since 1900
        .tm_mon = 6 - 1,        // Month, where 0 = january
        .tm_mday = 1,           // Day of the month
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0,
    };

    // Set the RTC time
    if (rtc_set_time(&timeinfo) != 0) {
        printf("Failed to set RTC time.\n");
        return 1;
    }

    printf("Time set to June 1, 2024.\n");

    // Set an alarm for 5 seconds later
    timeinfo.tm_sec += 5;
    rtc_set_alarm(&timeinfo, rtc_alarm_callback, NULL);

    // Keep the main thread running
    while (1) {
        thread_yield();
    }
}
