#include <stdio.h>
#include <errno.h>
#include "periph_cpu.h"
#include "event/periodic.h"

// Function to handle the RTC alarm
void rtc_alarm_callback(void *arg) {
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize RTC
    if (rtc_init() != 0) {
        printf("Failed to initialize RTC\n");
        return -1;
    }

    // Set date to June 1, 2024
    struct tm timeinfo;
    memset(&timeinfo, 0, sizeof(struct tm));
    timeinfo.tm_year = 2024 - 1900; // tm_year is years since 1900
    timeinfo.tm_mon = 6 - 1;        // tm_mon is months since January
    timeinfo.tm_mday = 1;
    if (rtc_set_time(&timeinfo) != 0) {
        printf("Failed to set date\n");
        return -1;
    }

    // Calculate alarm time (5 seconds from now)
    time_t now;
    rtc_get_time(&now);
    now += 5;
    struct tm *alarm_time = localtime(&now);

    // Set the RTC alarm
    rtc_alarm_t alarm = { .time = *alarm_time, .callback = rtc_alarm_callback, .arg = NULL };
    if (rtc_set_alarm(&alarm) != 0) {
        printf("Failed to set alarm\n");
        return -1;
    }

    // Loop to keep main active
    while (1) {
        // Sleep or perform other tasks
    }

    return 0;
}