#include <time.h>
#include "timex.h"
#include "rtc.h"

void alarm_callback(void *arg) {
    (void)arg;
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm alarm_time;
    rtc_alarm_t alarm;

    // Set the date to June 1, 2024.
    alarm_time.tm_year = 2024 - 1900; // Year since 1900
    alarm_time.tm_mon = 6 - 1;        // Month, where 0 = January
    alarm_time.tm_mday = 1;          // Day of the month
    alarm_time.tm_hour = 0;          // Hours since midnight
    alarm_time.tm_min = 0;           // Minutes after the hour
    alarm_time.tm_sec = 5;           // Seconds after the minute

    rtc_set_time(&alarm_time);

    // Set the alarm 5 seconds later
    rtc_get_time(&alarm_time);
    alarm_time.tm_sec += 5;

    rtc_set_alarm(&alarm_time, alarm_callback, NULL);

    while (1) {
        // Keep the program running
    }

    return 0;
}