#include <stdio.h>   // Standard input-output header
#include "periph/rtc.h"  // RTC peripheral to include rtc.h for RTC functionalities

void rtc_alarm_cb(void) {
    printf("Alarm triggered!\n");
}

/* Main program */
int main(void) {
    struct tm time;

    // Set date to June 1, 2024
    time.tm_year = 2024 - 1900;  // tm_year is years since 1900
    time.tm_mon = 5;             // tm_mon range is 0-11, so 5 is June
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    if (rtc_set_time(&time) != 0) {
        printf("Failed to set RTC time\n");
        return 1;
    }

    // Set RTC alarm for 5 seconds later
    rtc_set_alarm(&time, 5);  // Assuming there's a function to set alarm after seconds
    rtc_alarm_cb();

    return 0;
}