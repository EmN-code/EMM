#include <xtimer.h>
#include <periph/rtc.h>
#include <stdio.h>

int main(void) {
    // Initialize the RTC
    if (rtc_init() != 0) {
        printf("RTC initialization failed\n");
        return -1;
    }

    // Set the RTC time to June 1, 2024, 00:00:00
    struct tm time;
    time.tm_year = 2024 - 1900; // years since 1900
    time.tm_mon = 6 - 1;        // months since January, 0-11
    time.tm_mday = 1;           // day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;
    rtc_set_time(&time);

    // Set RTC alarm for 5 seconds later
    time.tm_sec += 5;
    rtc_set_alarm(&time);

    // Wait and check for the alarm trigger in a loop
    while (1) {
        if (rtc_alarm_check()) {
            printf("Alarm triggered!\n");
            break;
        }
        xtimer_sleep(1); // Sleep for 1 second
    }

    return 0;
}
