#include <stdio.h>
#include "rtc_alarm_cb_t.h"
#include "rtc.h"
#include "thread.h"

void alarm_callback(void *arg) {
    (void) arg;
    puts("Alarm triggered!");
}

int main(void) {
    struct tm timeinfo;

    // Set the date and time to June 1, 2024, 00:00:00
    timeinfo.tm_year = 2024 - 1900;  // tm_year is year since 1900
    timeinfo.tm_mon = 5;             // June is month 5 (0-based)
    timeinfo.tm_mday = 1;            // Day of the month
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;

    // Set RTC with the specified time
    if (rtc_set_time(&timeinfo) != 0) {
        puts("Failed to set RTC time!");
        return 1;
    }

    // Set an alarm to trigger 5 seconds from the set time
    timeinfo.tm_sec += 5;
    if (rtc_set_alarm(&timeinfo, alarm_callback, NULL) != 0) {
        puts("Failed to set RTC alarm!");
        return 1;
    }

    puts("RTC alarm set for 5 seconds later. Waiting for alarm...\n");

    // Enter main loop (or sleep to save power, etc.)
    while (1) {
        thread_sleep();
    }

    return 0;
}