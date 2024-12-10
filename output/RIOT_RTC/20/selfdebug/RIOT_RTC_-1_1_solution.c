#include <stdio.h>
#include "rtc_alarm.h"

void alarm_callback(void *arg) {
    printf("Alarm triggered!\n");
}

int main() {
    struct tm time;
    rtc_alarm_cb_t cb = alarm_callback;

    // Set the date to June 1, 2024
    time.tm_year = 2024 - 1900;  // Year since 1900
    time.tm_mon = 6 - 1;         // Month, where 0 = January
    time.tm_mday = 1;            // Day of the month
    time.tm_hour = 0;
    time.tm_min = 0;
    time.tm_sec = 0;

    rtc_set_time(&time);

    // Set an alarm for 5 seconds later
    time.tm_sec += 5;
    rtc_set_alarm(&time, cb, NULL);

    while (1) {
        // Simulate long running process
    }

    return 0;
}