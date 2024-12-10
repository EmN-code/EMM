#include <stdio.h>
#include <time.h>
#include "xtimer.h"
#include "periph/rtc.h"

void my_alarm_callback(void *arg) {
    (void)arg;  // Avoid unused parameter warning
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize RTC
    if (rtc_init() != 0) {
        printf("Failed to initialize RTC\n");
        return -1;
    }

    // Set the current time to June 1, 2024, 00:00:00
    struct tm time;
    time.tm_year = 2024 - 1900; // Year since 1900
    time.tm_mon = 5;            // Month, where 0 = January
    time.tm_mday = 1;           // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    if (rtc_set_time(&time) != 0) {
        printf("Failed to set RTC time\n");
        return -1;
    }

    // Set an alarm 5 seconds from now
    time_t alarm_time = rtc_get_time() + 5;
    struct tm *alarm_tm = gmtime(&alarm_time);

    if (rtc_set_alarm(alarm_tm, my_alarm_callback, NULL) != 0) {
        printf("Failed to set RTC alarm\n");
        return -1;
    }

    // Keep the program running to wait for the alarm
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}