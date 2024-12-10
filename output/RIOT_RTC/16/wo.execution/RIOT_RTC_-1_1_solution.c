#include "xtimer.h"
#include "timex.h"
#include "rtc.h"
#include "stdio.h"

void alarm_callback(void *arg) {
    (void)arg; // To silence unused parameter warning
    printf("Alarm triggered!\n");
}

int main(void) {
    struct tm alarm_time;
    rtc_set_cb(alarm_callback, NULL);

    // Set current date and time
    struct tm current_time = {
        .tm_year = 2024 - 1900, // Year since 1900
        .tm_mon = 5,            // 0-indexed (0 is January, 5 is June)
        .tm_mday = 1,           // Day of the month
        .tm_hour = 0,
        .tm_min = 0,
        .tm_sec = 0
    };
    rtc_set_time(&current_time);

    // Set the alarm to trigger after 5 seconds
    alarm_time = current_time;
    alarm_time.tm_sec += 5;

    rtc_set_alarm(&alarm_time, alarm_callback, NULL);
    
    // Keep the program running to allow the alarm to trigger
    while (1) {
        xtimer_sleep(1);
    }

    return 0;
}