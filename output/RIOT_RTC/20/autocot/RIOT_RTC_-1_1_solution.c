#include <stdio.h>
#include <time.h>

#include "rtc_driver.h"
#include "periph/rtc.h"

void rtc_alarm_cb(void *arg) {
    (void)arg;  // suppress unused parameter warning
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm alarm_time;
    rtc_alarm_cb_t callback = rtc_alarm_cb;

    // Set the date and time
    time_t now = time(NULL);
    struct tm *current_time = localtime(&now);

    current_time->tm_year = 2024 - 1900; // tm_year is the number of years since 1900
    current_time->tm_mon = 6 - 1;        // tm_mon is 0-based, January is 0
    current_time->tm_mday = 1;           // Day of the month
    current_time->tm_hour = 0;           // Midnight
    current_time->tm_min = 0;
    current_time->tm_sec = 0;

    // Update RTC to current date/time
    rtc_set_time(current_time);

    // Set alarm time to 5 seconds after current time
    alarm_time = *current_time;
    alarm_time.tm_sec += 5;

    // Initialize the RTC alarm
    rtc_set_alarm(&alarm_time, callback, NULL);

    while (1) {
        // Infinite loop to keep the program running
    }

    return 0;
}