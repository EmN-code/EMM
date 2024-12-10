#include <stdio.h>
#include <kernel_defines.h>
#include <time.h>
#include <mosfet/rtc.h>

static void alarm_cb(void *arg) {
    (void)arg; // Normally we would use this if needed
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm timeinfo = {
        .tm_year = 2024 - 1900, // Year since 1900
        .tm_mon = 6 - 1,        // Month, where 0 = January
        .tm_mday = 1,           // Day of the month
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };

    // Set RTC time
    if (rtc_set_time(&timeinfo) != 0) {
        printf("Failed to set RTC time\n");
        return 1;
    }

    // Set alarm to trigger in 5 seconds
    struct tm alarm_time = timeinfo;
    alarm_time.tm_sec += 5;

    if (rtc_set_alarm(&alarm_time, alarm_cb, NULL) != 0) {
        printf("Failed to set RTC alarm\n");
        return 1;
    }

    printf("RTC alarm set for 5 seconds from now\n");
    // Enter a busy loop (or sleep) to wait for alarm
    while(1);
    return 0;
}