#include "xtimer.h"
#include "timex.h"
#include "rtc.h"
#include <stdio.h>

void alarm_callback(void *arg) {
    (void)arg;  // to address unused parameter warning
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time;

    // Set the RTC to June 1, 2024, 00:00:00
    time.tm_year = 2024 - 1900; // Years since 1900
    time.tm_mon = 5;            // Month, 0 = January
    time.tm_mday = 1;           // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    if (rtc_init() != 0) {
        printf("Error initializing RTC\n");
        return 1;
    }

    if (rtc_set_time(&time) != 0) {
        printf("Error setting RTC time\n");
        return 1;
    }

    // Set an alarm for 5 seconds in the future
    xtimer_sleep(5);

    if (rtc_set_alarm(&time, alarm_callback, NULL) != 0) {
        printf("Error setting RTC alarm\n");
        return 1;
    }

    printf("RTC alarm set for 5 seconds from now\n");

    // Endless loop
    while (1) {
        // Main loop does nothing, just wait for the alarm to occur
    }

    return 0;
}