#include <stdio.h>
#include "rtc.h"
#include "thread.h"

void alarm_callback(void *arg) {
    (void)arg; // Avoid unused parameter warning.
    puts("Alarm triggered!");
}

int main(void) {
    struct tm time;
    rtc_alarm_cb_t callback = alarm_callback;

    // Set date to June 1, 2024.
    time.tm_year = 2024 - 1900; // Years since 1900
    time.tm_mon = 5;            // 0-11, so 5 for June
    time.tm_mday = 1;           // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    // Initialize RTC and set time
    if (rtc_init() < 0) {
        puts("RTC initialization failed.");
        return -1;
    }
    if (rtc_set_time(&time) < 0) {
        puts("Setting RTC time failed.");
        return -1;
    }

    // Set alarm for 5 seconds from now
    time.tm_sec += 5;
    if (rtc_set_alarm(&time, callback, NULL) < 0) {
        puts("Setting RTC alarm failed.");
        return -1;
    }

    // Keep the program running to catch alarm
    while (1) {
        thread_yield();
    }

    return 0;
}