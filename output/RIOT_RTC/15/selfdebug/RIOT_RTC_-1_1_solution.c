#include <stdio.h>
#include "xtimer.h"
#include "periph/rtc.h"

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    printf("RTC alarm example\n");

    struct tm time = {
        .tm_year = 2024 - 1900, // Year since 1900
        .tm_mon  = 5,           // Month [0-11, where 0 = January]
        .tm_mday = 1,           // Day of the month [1-31]
        .tm_hour = 0,
        .tm_min  = 0,
        .tm_sec  = 0
    };

    if (rtc_set_time(&time) != 0) {
        printf("Error setting RTC time\n");
        return 1;
    }

    // Add 5 seconds to the current time
    time.tm_sec += 5;

    // Normalize the time structure
    mktime(&time);

    if (rtc_set_alarm(&time, alarm_callback, NULL) != 0) {
        printf("Error setting RTC alarm\n");
        return 1;
    }

    printf("Alarm set for 5 seconds!\n");

    // Infinite loop to keep the program running
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}