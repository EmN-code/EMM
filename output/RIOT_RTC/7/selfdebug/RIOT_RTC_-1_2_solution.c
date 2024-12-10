#include <periph/rtc.h>
#include <stdio.h>

void rtc_callback(void) {
    printf("Alarm triggered!\n");
}

int main(void) {
    // Initialize RTC
    if (rtc_init() != 0) {
        printf("RTC initialization failed\n");
        return 1;
    }

    // Set the date to June 1, 2024
    struct tm time;
    time.tm_year = 2024 - 1900; // Year since 1900
    time.tm_mon = 5;  // Months since January
    time.tm_mday = 1; // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    if (rtc_set_time(&time) != 0) {
        printf("Failed to set RTC time\n");
        return 1;
    }

    // Set up an alarm for 5 seconds later
    struct tm alarm_time = time;
    alarm_time.tm_sec += 5;

    if (rtc_set_alarm(&alarm_time, rtc_callback) != 0) {
        printf("Setting alarm failed\n");
        return 1;
    }

    printf("Alarm set for 5 seconds from now\n");

    while (1) {
        // Main loop doing nothing, just waiting for the interrupt
    }

    return 0;
}