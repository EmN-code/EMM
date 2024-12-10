#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <periph_cpu.h>  // Including the correct header for RTC

void rtc_alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm timeinfo;
    rtc_init();

    // Set date to June 1, 2024
    timeinfo.tm_year = 2024 - 1900;  // Year since 1900
    timeinfo.tm_mon = 6 - 1;         // Month, 0-11
    timeinfo.tm_mday = 1;            // Day of the month
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;
    rtc_set_time(&timeinfo);
    printf("RTC time set to: 2024-06-01 00:00:00\n");

    // Set alarm for 5 seconds later
    rtc_alarm_set(rtc_get_time() + 5, rtc_alarm_callback, NULL);
    printf("RTC alarm set for 5 seconds from now\n");

    while (1) {
        // Main loop does nothing, alarm callback will handle the action
    }

    return 0;
}