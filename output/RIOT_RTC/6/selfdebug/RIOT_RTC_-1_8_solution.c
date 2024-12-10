#include <stdio.h>
#include "xtimer.h"
#include "periph/rtc.h"

void rtc_set_alarm_callback(void *arg)
{
    (void)arg; // To avoid compiler warnings of unused variable
    printf("Alarm triggered!\n");
}

int main(void)
{
    // Set date to June 1, 2024, 00:00:00
    struct tm time = {
        .tm_year = 2024 - 1900, // Year since 1900
        .tm_mon = 5, // Month (0-based; 0=January)
        .tm_mday = 1, // Day of the month
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };

    // Initialize RTC
    if (rtc_init() != 0) {
        printf("Failed to initialize RTC\n");
        return 1;
    }

    // Set RTC time
    if (rtc_set_time(&time) != 0) {
        printf("Failed to set RTC time\n");
        return 1;
    }

    // Schedule alarm 5 seconds later
    time.tm_sec += 5;
    if (rtc_set_alarm(&time, rtc_set_alarm_callback, NULL) != 0) {
        printf("Failed to set alarm\n");
        return 1;
    }

    printf("Alarm set for 5 seconds from set date\n");

    // Keep running to catch the alarm
    while (1) {
        xtimer_sleep(1); // Sleep to prevent busy wait, not required.
    }

    return 0;
}