#include <stdio.h>
#include "rtc.h"
#include "xtimer.h"

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    // Define the RTC time as June 1, 2024, 00:00:00
    struct tm time = {
        .tm_year = 2024 - 1900,  // Year since 1900
        .tm_mon = 5,            // June (MONTH is 0-11)
        .tm_mday = 1,
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0,
    };

    // Initialize RTC and set the time
    rtc_init();
    rtc_set_time(&time);

    // Set an RTC alarm for 5 seconds later
    time.tm_sec += 5;
    rtc_set_alarm(&time, alarm_callback, NULL);

    // Keep the program running
    while (1) {
        xtimer_sleep(1);
    }
    return 0;
}