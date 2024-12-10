
#include <stdio.h>
#include <time.h>
#include "xtimer.h"
#include "periph/rtc.h"

#define ALARM_DELAY_SEC (5)

void alarm_callback(void *arg) {
    (void)arg;
    puts("Alarm triggered!");
}

int main(void) {
    struct tm time;

    // Initialize RTC
    rtc_init();

    // Set the date to June 1, 2024
    time.tm_year = 2024 - 1900; // years since 1900
    time.tm_mon = 5;            // months since January (0-11)
    time.tm_mday = 1;           // day of the month (1-31)
    time.tm_hour = 0;           // hours since midnight (0-23)
    time.tm_min = 0;            // minutes after the hour (0-59)
    time.tm_sec = 0;            // seconds after the minute (0-60)
    time.tm_isdst = -1;         // Daylight Saving Time flag

    // Set the RTC time
    rtc_set_time(&time);

    // Set the alarm to trigger after 5 seconds
    time.tm_sec += ALARM_DELAY_SEC;
    rtc_set_alarm(&time, alarm_callback, NULL);

    // Main loop
    while (1) {
        xtimer_sleep(1); // Sleep for 1 second
    }

    return 0;
}
