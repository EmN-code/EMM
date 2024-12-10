#include <xtimer.h>
#include <stdio.h>
#include <time.h>
#include "periph/rtc.h"

void alarm_triggered(void)
{
    printf("Alarm triggered!\n");
}

int main(void)
{
    // Initialize RTC
    if (rtc_init() != 0) {
        printf("Failed to initialize RTC\n");
        return -1;
    }

    // Set the date to June 1, 2024, 12:00:00
    struct tm time;
    time.tm_year = 2024 - 1900;  // Years since 1900
    time.tm_mon = 5;             // Month, where 0 = jan
    time.tm_mday = 1;            // Day of the month
    time.tm_hour = 12;
    time.tm_min = 0;
    time.tm_sec = 0;

    if (rtc_set_time(&time) != 0) {
        printf("Failed to set time\n");
        return -1;
    }
    printf("RTC date set to June 1, 2024\n");

    // Set an alarm to trigger after 5 seconds
    struct tm alarm_time = time;
    alarm_time.tm_sec += 5;
    mktime(&alarm_time);  // Normalize the tm structure

    rtc_set_alarm(&alarm_time, alarm_triggered);
    printf("Alarm set for 5 seconds from now\n");

    // Main loop does nothing, alarm will trigger after 5 seconds
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}