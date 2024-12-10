#include <stdio.h>
#include "xtimer.h"
#include "periph/rtc.h"
#include "periph/gpio.h"

void rtc_alarm_cb(void *arg) {
    (void)arg;
    puts("Alarm triggered!");
}

int main(void) {
    struct tm time = {
        .tm_year = 2024 - 1900, // Year since 1900
        .tm_mon  = 6 - 1,       // Month, where 0 = jan
        .tm_mday = 1,           // Day of the month
        .tm_hour = 0,
        .tm_min  = 0,
        .tm_sec  = 0
    };

    struct tm alarm_time;

    if (rtc_init() != 0) {
        printf("Failed to initialize RTC\n");
        return 1;
    }

    if (rtc_set_time(&time) != 0) {
        printf("Failed to set RTC time\n");
        return 1;
    }

    xtimer_sleep(1);  // Ensure RTC time is set properly

    rtc_get_time(&alarm_time);  // Get the current time
    alarm_time.tm_sec += 5;  // Set the alarm for 5 seconds later

    if (rtc_set_alarm(&alarm_time, rtc_alarm_cb, NULL) != 0) {
        printf("Failed to set RTC alarm\n");
        return 1;
    }

    while (1) {
        xtimer_sleep(1);  // Sleep to reduce CPU usage
    }

    return 0;
}