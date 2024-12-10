#include <stdio.h>
#include "xtimer.h"
#include "timex.h"
#include "rtc.h"

void alarm_callback(void *arg) {
    (void)arg;  // avoid unused parameter warning
    puts("Alarm triggered!");
}

int main(void) {
    puts("Setting RTC date and alarm");

    // Define the time structure for June 1, 2024, 00:00:00
    struct tm time;
    time.tm_year = 2024 - 1900;  // Years since 1900
    time.tm_mon = 5;             // Months since January (0-11)
    time.tm_mday = 1;            // Day of the month (1-31)
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    // Set the RTC time
    if (rtc_set_time(&time) != 0) {
        puts("Failed to set RTC time");
        return 1;
    }

    // Configure the alarm to trigger in 5 seconds
    xtimer_sleep(5);

    // Set the RTC alarm
    if (rtc_set_alarm(&time, alarm_callback, NULL) != 0) {
        puts("Failed to set RTC alarm");
        return 1;
    }

    puts("Alarm set for 5 seconds from now.");

    // Loop to keep the program running
    while (1) {
        // Sleep to avoid busy waiting
        xtimer_sleep(1);
    }

    return 0;
}