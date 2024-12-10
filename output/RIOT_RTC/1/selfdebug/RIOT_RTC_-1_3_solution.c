#include <stdio.h>
#include <time.h>
#include "xtimer.h"
#include "rtc.h"

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm timeinfo;
    timeinfo.tm_year = 2024 - 1900;  // Years since 1900
    timeinfo.tm_mon = 5;             // Month, June = 5
    timeinfo.tm_mday = 1;            // Day of the month
    timeinfo.tm_hour = 0;
    timeinfo.tm_min = 0;
    timeinfo.tm_sec = 0;

    // Set the RTC time
    rtc_set_time(&timeinfo);

    // Add debug output to verify the initial date setting
    printf("Date set to: %d-%02d-%02d %02d:%02d:%02d\n",
           timeinfo.tm_year + 1900, timeinfo.tm_mon + 1, timeinfo.tm_mday,
           timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec);

    // Set an alarm to trigger after 5 seconds
    struct tm alarm_time = timeinfo;
    alarm_time.tm_sec += 5;  // Set alarm 5 seconds later

    // Adjust for overflow in seconds
    mktime(&alarm_time);

    // Set the RTC alarm
    rtc_set_alarm(&alarm_time, alarm_callback, NULL);

    // Wait and let the alarm trigger
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}