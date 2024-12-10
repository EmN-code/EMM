#include <stdio.h>
#include <time.h>
#include "periph/rtc.h"

void alarm_callback(void *arg) {
    (void) arg; // Mark as unused
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm date_time;
    date_time.tm_year = 2024 - 1900; // Years since 1900
    date_time.tm_mon = 5; // June (0-based, so 5 for June)
    date_time.tm_mday = 1; // 1st day
    date_time.tm_hour = 0;
    date_time.tm_min = 0;
    date_time.tm_sec = 0;

    // Initialize RTC
    if (rtc_init() != 0) {
        printf("Failed to initialize RTC\n");
        return 1;
    }

    // Set the date and time
    if (rtc_set_time(&date_time) != 0) {
        printf("Failed to set time\n");
        return 1;
    }

    // Set an alarm 5 seconds in the future
    struct tm alarm_time;
    rtc_get_time(&alarm_time);
    alarm_time.tm_sec += 5;
    time_t alarm_epoch = mktime(&alarm_time);

    // Register the callback for the alarm
    if (rtc_set_alarm(&alarm_epoch, alarm_callback, NULL) != 0) {
        printf("Failed to set alarm\n");
        return 1;
    }

    // Keep the program running to allow alarm callback to occur
    while (1) {
        // In a real application, low-power sleep would be used
    }

    return 0;
}
