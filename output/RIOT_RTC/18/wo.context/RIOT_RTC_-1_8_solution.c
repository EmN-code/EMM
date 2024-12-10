#include <stdio.h>  // For input/output operations
#include <stdlib.h> // For standard libraries including date functions
#include "xtimer.h"  // For timer operations in RIOT
#include "periph/rtc.h"  // For RTC operations

void rtc_alarm_handler(void *arg) {
    (void)arg; // Argument is unused
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize the RTC
    if (rtc_init() != 0) {
        printf("RTC initialization failed!\n");
        return 1;
    }

    // Set date to June 1, 2024
    struct tm time;
    time.tm_year = 2024 - 1900; // tm_year is years since 1900
    time.tm_mon = 5; // June (0-based, so 5 for June)
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    if (rtc_set_time(&time) != 0) {
        printf("Failed to set RTC time!\n");
        return 1;
    }

    // Set the RTC alarm for 5 seconds from now
    xtimer_sleep(5); // Wait for 5 seconds

    struct tm alarm;
    rtc_get_time(&alarm);
    alarm.tm_sec += 5;
    mktime(&alarm); // Ensure correct wrapping of minutes/hours if needed

    rtc_set_alarm(&alarm, rtc_alarm_handler, NULL);

    printf("Alarm set for 5 seconds later.\n");

    // Keep the main function running to allow the alarm to trigger
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}