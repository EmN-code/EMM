#include <stdio.h>
#include <time.h>
#include <rtc.h>

void alarm_callback(void *arg) {
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm time_info;
    rtc_alarm_cb_t alarm_cb = alarm_callback;

    // Set date to June 1, 2024
    time_info.tm_year = 2024 - 1900;  // year since 1900
    time_info.tm_mon = 5;             // month, where 0 = January
    time_info.tm_mday = 1;            // day of the month
    time_info.tm_hour = 0;
    time_info.tm_min = 0;
    time_info.tm_sec = 0;

    // Initialize the RTC with this time
    rtc_set_time(&time_info);

    // Configure an alarm for 5 seconds later
    rtc_set_alarm(time(NULL) + 5, alarm_cb, NULL);

    // Go into low-power mode or idle wait
    while (1) {
        // Placeholder for low-power mode or periodic wake-up
    }

    return 0;
}