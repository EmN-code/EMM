#include <stdio.h>
#include <xtimer.h>
#include <time.h>
#include <thread.h>
#include <periph/rtc.h>

void rtc_cb(void *arg) {
    (void)arg; // Unused parameter
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize RTC
    rtc_init();

    struct tm time;
    time.tm_year = 2024 - 1900; // tm_year is year since 1900
    time.tm_mon = 5;            // tm_mon is month since January (0-11)
    time.tm_mday = 1;           // Day of the month (1-31)
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    // Set the RTC time
    rtc_set_time(&time);

    // Set alarm to trigger after 5 seconds
    struct tm alarm_time = time;
    alarm_time.tm_sec += 5;

    if (rtc_set_alarm(&alarm_time, rtc_cb, NULL) != 0) {
        printf("Failed to set RTC alarm\n");
        return 1;
    }

    printf("RTC alarm set for 5 seconds later.\n");

    // Infinite loop to keep the program running
    while (1) {
        // Sleep the thread so that the CPU can be used by others
        xtimer_sleep(10);
    }

    return 0;
}