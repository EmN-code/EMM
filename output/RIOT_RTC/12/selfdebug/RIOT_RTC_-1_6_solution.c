#include <stdio.h>
#include "xtimer.h"
#include "periph/rtc.h"

static void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize RTC
    struct tm time = {
        .tm_year = 2024 - 1900,  // Year - 1900
        .tm_mon = 5,             // Month, where 0 = january
        .tm_mday = 1,            // Day of the month
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0,
    };

    if (rtc_set_time(&time) != 0) {
        printf("Error setting RTC time\n");
    }

    // Set alarm for 5 seconds into the future
    struct tm alarm_time = time;
    alarm_time.tm_sec += 5;   // Set alarm for 5 seconds later
    mktime(&alarm_time);  // Normalize time structure

    if (rtc_set_alarm(&alarm_time, alarm_callback, NULL) != 0) {
        printf("Error setting RTC alarm\n");
    }

    // Main loop doing nothing, waiting for the alarm
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}