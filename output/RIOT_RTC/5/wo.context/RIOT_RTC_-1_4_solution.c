#include <stdio.h>
#include "xtimer.h"
#include "periph/rtc.h"

void rtc_alarm_cb(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time = {
        .tm_year = 2024 - 1900, // Year since 1900
        .tm_mon = 6 - 1,        // Month, where 0 = january
        .tm_mday = 1,           // Day of the month
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };

    // Initialize RTC
    if (rtc_set_time(&time) != 0) {
        printf("Failed to set time\n");
        return 1;
    }

    // Set alarm for 5 seconds later
    struct tm alarm_time = time;
    alarm_time.tm_sec += 5; // Set 5 seconds from set time

    if (rtc_set_alarm(&alarm_time, rtc_alarm_cb, NULL) != 0) {
        printf("Failed to set alarm\n");
        return 1;
    }

    printf("RTC is set for June 1, 2024, with an alarm in 5 seconds.\n");

    // Let the program sleep; the alarm callback will wake it
    xtimer_sleep(10); // Sleep more than 5 seconds to ensure alarm triggers

    return 0;
}