#include <stdio.h>
#include <xtimer.h>
#include <time.h>
#include "periph/rtc.h"

// Callback function for the alarm
void alarm_callback(void *arg) {
    (void)arg;  // Unused parameter
    puts("Alarm triggered!");
}

int main(void) {
    // Initialize RTC
    if (rtc_init() != 0) {
        puts("Failed to initialize RTC");
        return 1;
    }

    // Set date to June 1, 2024
    struct tm time;
    time.tm_year = 2024 - 1900;  // Years since 1900
    time.tm_mon = 6 - 1;         // Months since January
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    if (rtc_set_time(&time) != 0) {
        puts("Failed to set RTC time");
        return 1;
    }

    // Set alarm for 5 seconds from now
    xtimer_sleep(5);
    if (rtc_set_alarm(&time, alarm_callback, NULL) != 0) {
        puts("Failed to set RTC alarm");
        return 1;
    }

    puts("Waiting for alarm...");

    // Keeping the main thread alive
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}