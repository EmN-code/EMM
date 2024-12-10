#include "time.h"
#include "rtc.h"
#include "stdio.h"

void rtc_alarm_callback(void *arg) {
    // This function will be called when the alarm triggers
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm date_time;

    // Set date to June 1, 2024
    date_time.tm_year = 2024 - 1900;  // tm_year is year since 1900
    date_time.tm_mon = 6 - 1;         // tm_mon is 0-based
    date_time.tm_mday = 1;
    date_time.tm_hour = 0;
    date_time.tm_min = 0;
    date_time.tm_sec = 0;

    // Initialize RTC
    if (rtc_init() != 0) {
        printf("Failed to initialize RTC\n");
        return -1;
    }

    // Set the RTC time
    if (rtc_set_time(&date_time) != 0) {
        printf("Failed to set RTC time\n");
        return -1;
    }

    // Set an alarm for 5 seconds from now
    date_time.tm_sec += 5;

    // Normalize the time structure (handle overflow in tm_sec)
    mktime(&date_time);

    if (rtc_set_alarm(&date_time, rtc_alarm_callback, NULL) != 0) {
        printf("Failed to set RTC alarm\n");
        return -1;
    }

    // Keeping the main function alive to wait for the alarm
    while (1) {
        // Here we just simulate some kind of sleep to keep the CPU idle
        __asm__ volatile("wfi");
    }
    return 0;
}