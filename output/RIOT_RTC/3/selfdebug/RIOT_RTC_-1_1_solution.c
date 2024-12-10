#include <stdio.h>
#include <rtc.h>
#include <kernel.h>
#include <thread.h>

void alarm_callback(void *arg) {
    (void)arg;  // Ignore unused parameter warning
    // Print message when alarm is triggered
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time;

    // Set initial time to June 1, 2024
    time.tm_year = 2024 - 1900;  // tm_year is years since 1900
    time.tm_mon = 6 - 1;         // tm_mon is months since January
    time.tm_mday = 1;
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    if (rtc_init() != 0) {
        printf("RTC initialization failed\n");
        return 1;
    }

    rtc_set_time(&time);

    // Advance time by 5 seconds to set alarm
    time.tm_sec += 5;

    if (rtc_set_alarm(&time, alarm_callback, NULL) != 0) {
        printf("Failed to set RTC alarm\n");
        return 1;
    }

    // Wait indefinitely
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}
